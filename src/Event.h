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

/* Do not call it "EVENT_H" since some apple header library uses it too ...
 * Same prob: http://lists.apple.com/archives/quicktime-api/2005/dec/msg00162.html
 */
#ifndef _EVENT_H_
#define _EVENT_H_

namespace Annoyme {

enum EventType {
  eventNormalKeyPressed,
  eventNormalKeyReleased,
  eventEnterKeyPressed,
  eventEnterKeyReleased,
  eventBackspaceKeyPressed,
  eventBackspaceKeyReleased,
  eventMousePressed,
  eventMouseReleased,
};

}

class Event
{
public:
  Event();
  virtual ~Event();

public:
  void setType(Annoyme::EventType new_var)
  {
    m_type = new_var;
  }

  Annoyme::EventType getType() {
    return m_type;
  }

  void setValue(string new_var)
  {
    m_value = new_var;
  }

  string getValue()
  {
    return m_value;
  }

  void setSymbol(unsigned int new_var)
  {
    m_symbol = new_var;
  }

  unsigned int getSymbol()
  {
    return m_symbol;
  }

private:
  Annoyme::EventType     m_type;
  string        m_value;
  unsigned int  m_symbol;
};

#endif // _EVENT_H_
