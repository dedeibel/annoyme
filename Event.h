
#ifndef EVENT_H
#define EVENT_H

class Event
{
public:
  Event();
  virtual ~Event();

public:
  void setType(EventType new_var)
  {
    m_type = new_var;
  }

  EventType getType() {
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

private:
  EventType m_type;
  string m_value;

};

#endif // EVENT_H
