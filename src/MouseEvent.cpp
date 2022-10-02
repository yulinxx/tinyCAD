#include "MouseEvent.h"

#include <iostream>
/////////////////////////////////////////////
MouseMoveEvent::MouseMoveEvent(double x, double y)
    : m_dX(x), m_dY(y)
{
}


/////////////////////////////////////////////

MouseScrolledEvent::MouseScrolledEvent(double xoffset, double yoffset)
    :m_dXOffset(xoffset), m_dYOffset(yoffset)
{
    
}

/////////////////////////////////////////////

MousePressEvent::MousePressEvent(int button, int action, int mods)
    : m_nBtn(button), m_nAct(action), m_nMods(mods)
{
}

/////////////////////////////////////////////

MouseReleaseEvent::MouseReleaseEvent(int button, int action, int mods)
    : m_nBtn(button), m_nAct(action), m_nMods(mods)
{
    
}