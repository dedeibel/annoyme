
#ifndef ANNOYME_H
#define ANNOYME_H

class Annoyme
{
public:
  Annoyme();

  virtual ~Annoyme();

  void normalKeyPressed();
  void normalKeyReleased();
  void enterPressed();
  void enterReleased();
  void backspacePressed();
  void backspaceReleased();
  void init();
  void run();
  void close();

private:
  Configuration     *config;
  InputEventReader  *input;
  SoundLoader       *soundLoader;
  SoundOutput       *soundOutput;
};

#endif // ANNOYME_H
