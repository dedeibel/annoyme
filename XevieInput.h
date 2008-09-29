
#ifndef XEVIEINPUT_H
#define XEVIEINPUT_H
#include "InputEventReader.h"

class XevieInput : virtual public InputEventReader
{
public:
  XevieInput();
  virtual ~XevieInput();
};

#endif // XEVIEINPUT_H
