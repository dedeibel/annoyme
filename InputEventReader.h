
#ifndef INPUTEVENTREADER_H
#define INPUTEVENTREADER_H

class InputEventReader
{
public:
  virtual void open();
  virtual void close();
  virtual void getNextEvent();
};

#endif // INPUTEVENTREADER_H
