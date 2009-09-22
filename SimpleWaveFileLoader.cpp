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

#include <string>
#include <iostream>
#include <map>
#include <cerrno>
#include <cstring>
#include <cstdlib>

using namespace std;

extern "C" {
  #include <glob.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <libgen.h>
}

#include "exceptions.h"
#include "Sample.h"

#include "SimpleWaveFileLoader.h"
#include "stdutil.h"

// TODO rename to raw file loader

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
  path += "/*.raw";

  glob_t globbuf;
  glob(path.c_str(), 0, 0, &globbuf);

  bool foundSamples = false;
  for (unsigned int i = 0; i < globbuf.gl_pathc; ++i)
  {
    foundSamples = true;
    loadSampleFromFile(globbuf.gl_pathv[i]);
  }

  if (foundSamples == 0) {
    cout << "Warning, no sound samples found in: '" << path << "'" << endl;
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
  if (type == Sample::invalidType)
  {
    cerr << "Invalid sample: " << name << endl;
    return;
  }
  
  Sample *sample = new Sample();
  
  try
  {
    sample->setName(name);
    sample->setFilePath(path);
    sample->setType(type);

    loadDataFromFile(sample);
   }
   catch (...)
   {
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

  // TODO parameterize
  unsigned int allocated = 32000 * sizeof(char);
  unsigned int bytesRead = 0;

  int file = open(sample->getFilePath().c_str(), O_RDONLY);
  if (file == 0) return;
  
  data = (char*)malloc(allocated);
  while ((bytesRead = read(file, data, allocated - size)))
  {
    size += bytesRead;
    if (size >= allocated) {
      allocated *= 2;
      data = (char*)realloc(data, allocated);
    }
    if (data == 0)
    {
      cerr << "allocation of more memory to read sample file '"
           << sample->getFilePath() << "' failed.\n";
      break;
    }
  }

  sample->setSize(size);
  sample->setData(data);
  cout << "   read sample '" << sample->getName() << "' with "
       << sample->getSize() << " Bytes.\n";
  
  close(file);
}
