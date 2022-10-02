#ifndef _KEY_EVENT_H
#define _KEY_EVENT_H

#include "Event.h"

/////////////////////////////////////////////
class KeyPressEvent : public Event
{
public:
    KeyPressEvent(int key, int scancode, int action, int mods);
    virtual ~KeyPressEvent() = default;
    
public:

private:
    int m_nKey = 0;
    int m_nScancode = 0;
    int m_nAction = 0;
    int m_nMods = 0;

};

/////////////////////////////////////////////

class KeyReleaseEvent : public Event
{
public:
    KeyReleaseEvent(int key, int scancode, int action, int mods);
    virtual ~KeyReleaseEvent() = default;
    
public:

private:
    int m_nKey = 0;
    int m_nScancode = 0;
    int m_nAction = 0;
    int m_nMods = 0;
};

#endif //_KEY_EVENT_H
