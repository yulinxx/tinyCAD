#ifndef _RESIZE_EVENT_H_
#define _RESIZE_EVENT_H_

#include "Event.h"

/////////////////////////////////////////////

class ResizeEvent : public Event
{
public:
    ResizeEvent(int w, int h);
    virtual ~ResizeEvent() = default;

private:
    int m_nW = 0;
    int m_nH = 0;

};

/////////////////////////////////////////////

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;
    virtual ~WindowCloseEvent() = default;

public:

private:

};
#endif //_RESIZE_EVENT_H_