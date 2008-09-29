
#ifndef SAMPLE_H
#define SAMPLE_H

class Sample
{
public:
  Sample();
  virtual ~Sample();
public:
  void setType (SampleType new_var)
  {
    m_type = new_var;
  }

  SampleType getType()
  {
    return m_type;
  }

  void setRate(int new_var)
  {
    m_rate = new_var;
  }

  int getRate()
  {
    return m_rate;
  }

  void setData(char* new_var)
  {
    m_data = new_var;
  }

  char* getData()
  {
    return m_data;
  }

  void setName(SoundSample new_var)
  {
    m_name = new_var;
  }

  SoundSample getName()
  {
    return m_name;
  }

private:
  SampleType  m_type;
  int         m_rate;
  char*       m_data;
  SoundSample m_name;
};

#endif // SAMPLE_H
