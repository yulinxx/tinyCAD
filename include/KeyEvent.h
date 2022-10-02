#ifndef _KEY_EVENT_H
#define _KEY_EVENT_H

#include "Event.h"

/////////////////////////////////////////////
class KeyPressEvent : public Event
{
public:
    KeyPressEvent() = default;
    virtual ~KeyPressEvent() = default;
    
public:

};

/////////////////////////////////////////////

class KeyReleaseEvent : public Event
{
public:
    KeyReleaseEvent() = default;
    virtual ~KeyReleaseEvent() = default;
    
public:

};
#endif //_KEY_EVENT_H
