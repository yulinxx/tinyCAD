#ifndef _MOUSE_EVENT_H_
#define _MOUSE_EVENT_H_

#include "Event.h"

/////////////////////////////////////////////
class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(double x, double y);
    virtual ~ MouseMoveEvent() = default;

private:
    double m_dX = 0.0;
    double m_dY = 0.0;

};


/////////////////////////////////////////////

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(double xoffset, double yoffset);
    virtual ~MouseScrolledEvent() = default;

public:

private:
    double m_dXOffset = 0.0;
    double m_dYOffset = 0.0;


};

/////////////////////////////////////////////

class MousePressEvent : public Event
{
public:
    MousePressEvent(int button, int action, int mods);
    virtual ~MousePressEvent() = default;

public:

private:
    int m_nBtn = 0;
    int m_nAct = 0;
    int m_nMods = 0;
};

/////////////////////////////////////////////
class MouseReleaseEvent : public Event
{
public:
    MouseReleaseEvent(int button, int action, int mods);
    virtual ~MouseReleaseEvent() = default;

public:

private:
    int m_nBtn = 0;
    int m_nAct = 0;
    int m_nMods = 0;
};
#endif //_MOUSE_EVENT_H_