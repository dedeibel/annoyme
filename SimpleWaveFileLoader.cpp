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

#include "stdheaders.h"
#include <glob.h>

#include "exceptions.h"
#include "Sample.h"

#include "SimpleWaveFileLoader.h"
#include "stdutil.h"

bool isDirectory(const string &path)
{
  struct stat buf;
  int retval = stat(path.c_str(), &buf);
  if (retval == -1)
  {
    throw AnnoyErrnoException("Could not stat directory", path, errno);
  }

  if (S_ISDIR(buf.st_mode))
  {
    return true;
  }
  else
  {
    return false;
  }
}

SimpleWaveFileLoader::SimpleWaveFileLoader(const string &path)
: m_path(path)
{
  insertDefault();
}

SimpleWaveFileLoader::~SimpleWaveFileLoader()
{
  this->clear();
}

void SimpleWaveFileLoader::clear()
{
  for (map<const enum Sample::SampleType, Sample*>::const_iterator p = samples.begin();
       p != samples.end();
       p++
  ) {
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

void SimpleWaveFileLoader::loadFiles()
{
  this->clear();
  insertDefault();

  if (! isDirectory(m_path)) return;
  string path = m_path;
  path += "/*.wav";

  glob_t globbuf;
  glob(path.c_str(), 0, 0, &globbuf);
  for (unsigned int i = 0; i < globbuf.gl_pathc; ++i)
  {
    loadSampleFromFile(globbuf.gl_pathv[i]);
  }
  cout << "done.\n";
  globfree(&globbuf);
}


void SimpleWaveFileLoader::getSample(enum Sample::SampleType type, const Sample **sample)
{
  Sample *foundSample = samples.find(type)->second;
  if (foundSample == 0)
  {
    *sample = samples.find(Sample::defaultType)->second;
  }
  else {
    *sample = foundSample;
  }
}

void SimpleWaveFileLoader::loadSampleFromFile(const char *path)
{
    const string name(this->getName(path));
    const enum Sample::SampleType type = Sample::getSampleType(name);
    if (type != Sample::invalidType)
    {
      Sample *sample = new Sample();
      sample->setName(name);
      sample->setFilePath(path);
      sample->setType(Sample::getSampleType(name));
      samples.insert(make_pair(sample->getType(), sample));
    }
    else {
      cout << "Invalid sample: " << name << endl;
    }
}

void loadDataFromFile(const char *path, char *&data, unsigned int &size)
{

}
