#include "KeyEvent.h"

/////////////////////////////////////////////

KeyPressEvent::KeyPressEvent(int key, int scancode, int action, int mods)
    : m_nKey(key), m_nScancode(scancode), m_nAction(action), m_nMods(mods)
{
    
}

/////////////////////////////////////////////

KeyReleaseEvent::KeyReleaseEvent(int key, int scancode, int action, int mods)
    : m_nKey(key), m_nScancode(scancode), m_nAction(action), m_nMods(mods)
{
    
}