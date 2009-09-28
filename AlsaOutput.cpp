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

extern "C" {
  #include <alsa/asoundlib.h>
}

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
  int pcmreturn;
  snd_pcm_state_t state = snd_pcm_state(m_pcm_handle);
  string state_s = "";
  switch (state)
  {
    case SND_PCM_STATE_OPEN: state_s = "Open"; break;
    case SND_PCM_STATE_SETUP:	state_s = "Setup installed"; break;
    case SND_PCM_STATE_PREPARED: state_s = "Ready to start"; break;
    case SND_PCM_STATE_RUNNING: state_s = "Running"; break;
    case SND_PCM_STATE_XRUN: state_s = "Stopped: underrun (playback) or overrun (capture) detected"; break;
    case SND_PCM_STATE_DRAINING: state_s = "Draining: running (playback) or stopped (capture)"; break;
    default: state_s = "bla"; break;

  }
  snd_pcm_sframes_t frames = snd_pcm_bytes_to_frames(m_pcm_handle, sample->getSize());
  while ((pcmreturn = snd_pcm_writei(m_pcm_handle, sample->getData(), frames)) < 0)
  {
    cerr << "pcmreturn: " << pcmreturn << " msg: " <<  snd_strerror(pcmreturn) << endl;
    snd_pcm_prepare(m_pcm_handle);
  }
}

void AlsaOutput::open()
{
  /* Allocate the snd_pcm_hw_params_t structure */
  snd_pcm_hw_params_alloca(&m_hwparams);

  /* Open PCM. The last parameter of this function is the mode. */
  /* If this is set to 0, the standard mode is used. Possible   */
  /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */ 
  /* If SND_PCM_NONBLOCK is used, read / write access to the    */
  /* PCM device will return immediately. If SND_PCM_ASYNC is    */
  /* specified, SIGIO will be emitted whenever a period has     */
  /* been completely processed by the soundcard.                */
  if (snd_pcm_open(&m_pcm_handle, m_device.c_str(), SND_PCM_STREAM_PLAYBACK, 0) < 0)
  {
    throw AlsaOutputException(
            string("ALSA output: could not open output device: ") + m_device);
  }

  /* Init hwparams with full configuration space */
  if (snd_pcm_hw_params_any(m_pcm_handle, m_hwparams) < 0)
  {
    throw AlsaOutputException("ALSA output: Can not configure this PCM device.");
  }

  unsigned int channels = 1;
  unsigned int rate = 22050; /* Sample rate, alt. 22050, 44100 */
  unsigned int exact_rate;   /* Sample rate returned by */
                    /* snd_pcm_hw_params_set_rate_near */ 
  //int dir;          /* exact_rate == rate --> dir = 0 */
                    /* exact_rate < rate  --> dir = -1 */
                    /* exact_rate > rate  --> dir = 1 */
  int periods = 2;       /* Number of periods */
  // Two periods a 8192 for 16 * 1024
 // snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */
  snd_pcm_uframes_t periodsize = 256; /* Periodsize (bytes) */
  
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
  if (snd_pcm_hw_params_set_channels(m_pcm_handle, m_hwparams, channels) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting channels.");
  }
  
  snd_pcm_uframes_t periodsize_exact = periodsize;
  if (snd_pcm_hw_params_set_period_size_near(m_pcm_handle, m_hwparams, &periodsize_exact, NULL) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting period size.");
  }

  if (periodsize != periodsize_exact)
  {
    cerr << "Warning: The period size "<< periodsize <<"B is not supported by your hardware.\n"
                     "==> Using "<< periodsize_exact <<"B instead.\n";
  }
  
  snd_pcm_uframes_t buffer_size = (periodsize_exact * periods) << 1;

  /* Set buffer size (in frames). The resulting latency is given by */
  /* latency = periodsize * periods / (rate * bytes_per_frame)     */
  snd_pcm_uframes_t buffer_size_exact = buffer_size;
  if (snd_pcm_hw_params_set_buffer_size_near(m_pcm_handle, m_hwparams, &buffer_size_exact) < 0)
  {
    throw AlsaOutputException("ALSA output: Error setting buffersize.");
  }

  if (buffer_size_exact != buffer_size)
  {
    cerr << "Warning: The buffer size "<< buffer_size <<"B is not supported by your hardware.\n"
                     "==> Using "<< buffer_size_exact <<"B instead.\n";
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
  err = snd_pcm_sw_params_set_start_threshold(m_pcm_handle, m_swparams, buffer_size_exact - periodsize_exact);
  if (err < 0)
  {
    throw AlsaOutputException(string("Unable to set start threshold mode for playback: ") + snd_strerror(err));
  }

  /*
   * This stays here as an reminder for stupid behaviour ... I guess the reason why there
   * is so little documentation out there - people aren't reading it anyway.
   *
   * stop_threshold does not stop the playback if there is not enough data ... it stops it
   * when there is too much data. So this was pretty much the cause why I have overruns all
   * the time.
   *
   * err = snd_pcm_sw_params_set_stop_threshold(m_pcm_handle, m_swparams, periodsize_exact);
   * if (err < 0)
   * {
   *   throw AlsaOutputException(string("Unable to set stop threshold mode for playback: ") + snd_strerror(err));
   * }
   */

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
