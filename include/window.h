#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
class GLFWwindow;

class window
{
public:
    window() = default;
    ~window() = default;

public:
    bool initWnd(int w, int h, std::string& strName);

    GLFWwindow* getWnd(){return m_pWnd;}

    bool run();
    
private:
    GLFWwindow* m_pWnd = nullptr;
};

#endif //_WINDOW_H_