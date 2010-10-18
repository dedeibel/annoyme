/*
 * Copyright (c) 2008, Benjamin Peter <BenjaminPeter@arcor.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Benjamin Peter ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Benjamin Peter BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

extern "C"
{
#include <libgen.h>
}

#include <string>
#include <iostream>
#include <map>
#include <cerrno>
#include <cstring>
#include <cstdlib>

using namespace std;

#include "exceptions.h"
#include "Sample.h"

#include "SimpleWaveFileLoader.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AnnoymeConfiguration.h"

#include <vector>
#include "ResourceLoader.h"
#include <algorithm>

SimpleWaveFileLoader::SimpleWaveFileLoader(ResourceLoader *resourceLoader) :
	m_resourceLoader(resourceLoader)
{
	insertDefault();
}

SimpleWaveFileLoader::~SimpleWaveFileLoader()
{
	this->clear();
}

void SimpleWaveFileLoader::clear()
{
	for (map<const enum Sample::SampleType, Sample*>::const_iterator p =
			samples.begin(); p != samples.end(); p++) {
		delete p->second;
	}
	samples.clear();
}

void SimpleWaveFileLoader::insertDefault()
{
	Sample *sample = new Sample();
	sample->setName("default");
	sample->setFilePath("default");
	sample->setType(Sample::defaultType);
	samples.insert(make_pair(sample->getType(), sample));
}

void SimpleWaveFileLoader::loadFiles(const string& theme)
{
	this->clear();
	insertDefault();

	vector<string> sampleResources;
	m_resourceLoader->listResources(AnnoymeConfiguration::value("samples_dir")
			+ "/" + theme, sampleResources);
	if (sampleResources.empty()) {
		// TODO proper handling
		cout << "Warning, no sound samples found for theme '" << theme << "'"
				<< endl;
	}
	else {
		vector<string>::iterator it = sampleResources.begin();
		while (it != sampleResources.end()) {
			string suffix = it->substr(it->size() - 4);
			std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
			if (suffix == ".raw") {
				loadSampleFromFile(*it);
			}
			++it;
		}
	}
	cout << "done.\n";
}

void SimpleWaveFileLoader::getSample(enum Sample::SampleType type,
		const Sample **sample)
{
	SamplesMap::iterator foundSample = samples.find(type);
	if (foundSample == samples.end()) {
		*sample = samples.find(Sample::defaultType)->second;
	}
	else {
		*sample = foundSample->second;
	}
}

void SimpleWaveFileLoader::loadSampleFromFile(const string &path)
{
	const string name(this->getName(path));
	const enum Sample::SampleType type = Sample::getSampleType(name);
	if (type == Sample::invalidType) {
		cerr << "Invalid sample: " << name << endl;
		return;
	}

	Sample *sample = new Sample();

	try {
		sample->setName(name);
		sample->setFilePath(path);
		sample->setType(type);

		loadDataFromFile(sample);
	}
	catch (...) {
		delete sample;
		throw;
	}

	samples.insert(make_pair(sample->getType(), sample));
}

void SimpleWaveFileLoader::loadDataFromFile(Sample *sample)
{
	// TODO parameterize
	sample->setFormat(Sample::PCM);
	sample->setRate(22050);

	unsigned int size = 0;
	char* data = 0;
	m_resourceLoader->getContent(sample->getFilePath(), &data, &size);

	sample->setSize(size);
	sample->setData(data);
	cout << "   read sample '" << sample->getName() << "' with "
			<< sample->getSize() << " Bytes.\n";
}

