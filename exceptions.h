
#include <exception>

class AnnoymeException : public exception
{
  public:

  virtual const char* what() const throw()
  {
    return message.c_str();
  }


  private:

  string message;
};

class UnknownOptionException :: public exception
{
  public:

  UnknownOptionException(string name)
  : name(name)
  , message(string("Unknown option requested: ") + name)
  {
  }

  private:

  string name;
};
