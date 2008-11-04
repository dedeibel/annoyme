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
#include <alsa/asoundlib.h>

using namespace std;

#include "exceptions.h"
#include "Sample.h"

#include "AlsaOutput.h"


AlsaOutput::AlsaOutput(const std::string &device)
: m_device(device)
{

}

AlsaOutput::~AlsaOutput()
{

}

void AlsaOutput::playSound(const Sample *sample)
{
  std::cout << "Playing sound " << sample->getName() << std::endl;
  int pcmreturn;
  unsigned int frames = sample->getSize() >> 2;
  while ((pcmreturn = snd_pcm_writei(m_pcm_handle, sample->getData(), frames)) < 0)
  {
    cerr << "pcmreturn: " << pcmreturn << " msg: " <<  snd_strerror(pcmreturn) << endl;
    snd_pcm_drop(m_pcm_handle);
    snd_pcm_prepare(m_pcm_handle);
  }
}

void AlsaOutput::open()
{
  /* Allocate the snd_pcm_hw_params_t structure on the stack. */
  snd_pcm_hw_params_alloca(&m_hwparams);

  /* Open PCM. The last parameter of this function is the mode. */
  /* If this is set to 0, the standard mode is used. Possible   */
  /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */ 
  /* If SND_PCM_NONBLOCK is used, read / write access to the    */
  /* PCM device will return immediately. If SND_PCM_ASYNC is    */
  /* specified, SIGIO will be emitted whenever a period has     */
  /* been completely processed by the soundcard.                */
  if (snd_pcm_open(&m_pcm_handle, m_device.c_str(), m_stream, SND_PCM_NONBLOCK) < 0)
  {
    throw AlsaOutputException(
            string("ALSA output: could not open output device: ") + m_device);
  }

  /* Init hwparams with full configuration space */
  if (snd_pcm_hw_params_any(m_pcm_handle, m_hwparams) < 0)
  {
    throw AlsaOutputException("ALSA output: Can not configure this PCM device.");
  }

  unsigned int rate = 11025; /* Sample rate, alt. 22050, 44100 */
  unsigned int exact_rate;   /* Sample rate returned by */
                    /* snd_pcm_hw_params_set_rate_near */ 
  //int dir;          /* exact_rate == rate --> dir = 0 */
                    /* exact_rate < rate  --> dir = -1 */
                    /* exact_rate > rate  --> dir = 1 */
  int periods = 2;       /* Number of periods */
  //snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */
  snd_pcm_uframes_t periodsize = 12220; /* Periodsize (bytes) */
  
  /* Set access type. This can be either    */
  /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
  /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
  /* There are also access types for MMAPed */
  /* access, but this is beyond the scope   */
  /* of this introduction.                  */
  if (snd_pcm_hw_params_set_access(m_pcm_handle, m_hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting access.");
  }
  
  /* Set sample format */
  if (snd_pcm_hw_params_set_format(m_pcm_handle, m_hwparams, SND_PCM_FORMAT_S16_LE) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting format.");
  }
  
  /* Set sample rate. If the exact rate is not supported */
  /* by the hardware, use nearest possible rate.         */ 
  exact_rate = rate;
  if (snd_pcm_hw_params_set_rate_near(m_pcm_handle, m_hwparams, &exact_rate, 0) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting rate.");
  }

  if (rate != exact_rate)
  {
    cerr << "Warning: The rate "<< rate <<" Hz is not supported by your hardware.\n"
                     "==> Using "<< exact_rate <<" Hz instead.\n";
  }
  
  /* Set number of channels */
  if (snd_pcm_hw_params_set_channels(m_pcm_handle, m_hwparams, 2) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting channels.");
  }
  
  /* Set number of periods. Periods used to be called fragments. */ 
  if (snd_pcm_hw_params_set_periods(m_pcm_handle, m_hwparams, periods, 0) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting periods.");
  }
  
  /* Set buffer size (in frames). The resulting latency is given by */
  /* latency = periodsize * periods / (rate * bytes_per_frame)     */
  if (snd_pcm_hw_params_set_buffer_size(m_pcm_handle, m_hwparams, (periodsize * periods)>>2) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting buffersize.");
  }
  
   /* Apply HW parameter settings to */
  /* PCM device and prepare device  */
  if (snd_pcm_hw_params(m_pcm_handle, m_hwparams) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting HW params.");
  }

  snd_pcm_sw_params_alloca(&m_swparams);

  int err;
  /* get the current swparams */
  err = snd_pcm_sw_params_current(m_pcm_handle, m_swparams);
  if (err < 0)
  {
    throw AlsaOutputException(string("ALSA output: Unable to determine current swparams for playback: ") + snd_strerror(err));
  }


  /* start the transfer when the buffer is almost full: */
  /* (buffer_size / avail_min) * avail_min */
  err = snd_pcm_sw_params_set_start_threshold(m_pcm_handle, m_swparams, 1);
  if (err < 0)
  {
    throw AlsaOutputException(string("Unable to set start threshold mode for playback: ") + snd_strerror(err));
  }

  err = snd_pcm_sw_params_set_stop_threshold(m_pcm_handle, m_swparams, 0);
  if (err < 0)
  {
    throw AlsaOutputException(string("Unable to set stop threshold mode for playback: ") + snd_strerror(err));
  }

  /* write the parameters to the playback device */
  err = snd_pcm_sw_params(m_pcm_handle, m_swparams);
  if (err < 0)
  {
    throw AlsaOutputException(string("Unable to set sw params for playback: ") + snd_strerror(err));
  }
}

void AlsaOutput::close()
{

}
