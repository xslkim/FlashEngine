/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCGLView.h"
#include "base/CCDirector.h"
#include "base/CCTouch.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventMouse.h"
#include "base/CCIMEDispatcher.h"
#include "base/ccUtils.h"

#include <unordered_map>

NS_CC_BEGIN

// GLFWEventHandler

class GLFWEventHandler
{
public:
    static void onGLFWError(int errorID, const char* errorDesc)
    {
        if (_view)
            _view->onGLFWError(errorID, errorDesc);
    }

    static void onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
    {
        if (_view)
            _view->onGLFWMouseCallBack(window, button, action, modify);
    }

    static void onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseMoveCallBack(window, x, y);
    }

    static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseScrollCallback(window, x, y);
    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (_view)
            _view->onGLFWKeyCallback(window, key, scancode, action, mods);
    }

    static void onGLFWCharCallback(GLFWwindow* window, unsigned int character)
    {
        if (_view)
            _view->onGLFWCharCallback(window, character);
    }

    static void onGLFWWindowPosCallback(GLFWwindow* windows, int x, int y)
    {
        if (_view)
            _view->onGLFWWindowPosCallback(windows, x, y);
    }

    static void onGLFWframebuffersize(GLFWwindow* window, int w, int h)
    {
        if (_view)
            _view->onGLFWframebuffersize(window, w, h);
    }

    static void onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
    {
        if (_view)
            _view->onGLFWWindowSizeFunCallback(window, width, height);
    }

    static void setGLView(GLView* view)
    {
        _view = NULL;
    }

private:
    static GLView* _view;
};

GLView* GLFWEventHandler::_view = nullptr;

////////////////////////////////////////////////////

struct keyCodeItem
{
    int glfwKeyCode;
    EventKeyboard::KeyCode keyCode;
};

static std::unordered_map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
    /* The unknown key */
    { GLFW_KEY_UNKNOWN         , EventKeyboard::KeyCode::KEY_NONE          },

    /* Printable keys */
    { GLFW_KEY_SPACE           , EventKeyboard::KeyCode::KEY_SPACE         },
    { GLFW_KEY_APOSTROPHE      , EventKeyboard::KeyCode::KEY_APOSTROPHE    },
    { GLFW_KEY_COMMA           , EventKeyboard::KeyCode::KEY_COMMA         },
    { GLFW_KEY_MINUS           , EventKeyboard::KeyCode::KEY_MINUS         },
    { GLFW_KEY_PERIOD          , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_SLASH           , EventKeyboard::KeyCode::KEY_SLASH         },
    { GLFW_KEY_0               , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_1               , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_2               , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_3               , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_4               , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_5               , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_6               , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_7               , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_8               , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_9               , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_SEMICOLON       , EventKeyboard::KeyCode::KEY_SEMICOLON     },
    { GLFW_KEY_EQUAL           , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_A               , EventKeyboard::KeyCode::KEY_A             },
    { GLFW_KEY_B               , EventKeyboard::KeyCode::KEY_B             },
    { GLFW_KEY_C               , EventKeyboard::KeyCode::KEY_C             },
    { GLFW_KEY_D               , EventKeyboard::KeyCode::KEY_D             },
    { GLFW_KEY_E               , EventKeyboard::KeyCode::KEY_E             },
    { GLFW_KEY_F               , EventKeyboard::KeyCode::KEY_F             },
    { GLFW_KEY_G               , EventKeyboard::KeyCode::KEY_G             },
    { GLFW_KEY_H               , EventKeyboard::KeyCode::KEY_H             },
    { GLFW_KEY_I               , EventKeyboard::KeyCode::KEY_I             },
    { GLFW_KEY_J               , EventKeyboard::KeyCode::KEY_J             },
    { GLFW_KEY_K               , EventKeyboard::KeyCode::KEY_K             },
    { GLFW_KEY_L               , EventKeyboard::KeyCode::KEY_L             },
    { GLFW_KEY_M               , EventKeyboard::KeyCode::KEY_M             },
    { GLFW_KEY_N               , EventKeyboard::KeyCode::KEY_N             },
    { GLFW_KEY_O               , EventKeyboard::KeyCode::KEY_O             },
    { GLFW_KEY_P               , EventKeyboard::KeyCode::KEY_P             },
    { GLFW_KEY_Q               , EventKeyboard::KeyCode::KEY_Q             },
    { GLFW_KEY_R               , EventKeyboard::KeyCode::KEY_R             },
    { GLFW_KEY_S               , EventKeyboard::KeyCode::KEY_S             },
    { GLFW_KEY_T               , EventKeyboard::KeyCode::KEY_T             },
    { GLFW_KEY_U               , EventKeyboard::KeyCode::KEY_U             },
    { GLFW_KEY_V               , EventKeyboard::KeyCode::KEY_V             },
    { GLFW_KEY_W               , EventKeyboard::KeyCode::KEY_W             },
    { GLFW_KEY_X               , EventKeyboard::KeyCode::KEY_X             },
    { GLFW_KEY_Y               , EventKeyboard::KeyCode::KEY_Y             },
    { GLFW_KEY_Z               , EventKeyboard::KeyCode::KEY_Z             },
    { GLFW_KEY_LEFT_BRACKET    , EventKeyboard::KeyCode::KEY_LEFT_BRACKET  },
    { GLFW_KEY_BACKSLASH       , EventKeyboard::KeyCode::KEY_BACK_SLASH    },
    { GLFW_KEY_RIGHT_BRACKET   , EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
    { GLFW_KEY_GRAVE_ACCENT    , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_1         , EventKeyboard::KeyCode::KEY_GRAVE         },
    { GLFW_KEY_WORLD_2         , EventKeyboard::KeyCode::KEY_NONE          },

    /* Function keys */
    { GLFW_KEY_ESCAPE          , EventKeyboard::KeyCode::KEY_ESCAPE        },
    { GLFW_KEY_ENTER           , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_TAB             , EventKeyboard::KeyCode::KEY_TAB           },
    { GLFW_KEY_BACKSPACE       , EventKeyboard::KeyCode::KEY_BACKSPACE     },
    { GLFW_KEY_INSERT          , EventKeyboard::KeyCode::KEY_INSERT        },
    { GLFW_KEY_DELETE          , EventKeyboard::KeyCode::KEY_DELETE        },
    { GLFW_KEY_RIGHT           , EventKeyboard::KeyCode::KEY_RIGHT_ARROW   },
    { GLFW_KEY_LEFT            , EventKeyboard::KeyCode::KEY_LEFT_ARROW    },
    { GLFW_KEY_DOWN            , EventKeyboard::KeyCode::KEY_DOWN_ARROW    },
    { GLFW_KEY_UP              , EventKeyboard::KeyCode::KEY_UP_ARROW      },
    { GLFW_KEY_PAGE_UP         , EventKeyboard::KeyCode::KEY_KP_PG_UP      },
    { GLFW_KEY_PAGE_DOWN       , EventKeyboard::KeyCode::KEY_KP_PG_DOWN    },
    { GLFW_KEY_HOME            , EventKeyboard::KeyCode::KEY_KP_HOME       },
    { GLFW_KEY_END             , EventKeyboard::KeyCode::KEY_END           },
    { GLFW_KEY_CAPS_LOCK       , EventKeyboard::KeyCode::KEY_CAPS_LOCK     },
    { GLFW_KEY_SCROLL_LOCK     , EventKeyboard::KeyCode::KEY_SCROLL_LOCK   },
    { GLFW_KEY_NUM_LOCK        , EventKeyboard::KeyCode::KEY_NUM_LOCK      },
    { GLFW_KEY_PRINT_SCREEN    , EventKeyboard::KeyCode::KEY_PRINT         },
    { GLFW_KEY_PAUSE           , EventKeyboard::KeyCode::KEY_PAUSE         },
    { GLFW_KEY_F1              , EventKeyboard::KeyCode::KEY_F1            },
    { GLFW_KEY_F2              , EventKeyboard::KeyCode::KEY_F2            },
    { GLFW_KEY_F3              , EventKeyboard::KeyCode::KEY_F3            },
    { GLFW_KEY_F4              , EventKeyboard::KeyCode::KEY_F4            },
    { GLFW_KEY_F5              , EventKeyboard::KeyCode::KEY_F5            },
    { GLFW_KEY_F6              , EventKeyboard::KeyCode::KEY_F6            },
    { GLFW_KEY_F7              , EventKeyboard::KeyCode::KEY_F7            },
    { GLFW_KEY_F8              , EventKeyboard::KeyCode::KEY_F8            },
    { GLFW_KEY_F9              , EventKeyboard::KeyCode::KEY_F9            },
    { GLFW_KEY_F10             , EventKeyboard::KeyCode::KEY_F10           },
    { GLFW_KEY_F11             , EventKeyboard::KeyCode::KEY_F11           },
    { GLFW_KEY_F12             , EventKeyboard::KeyCode::KEY_F12           },
    { GLFW_KEY_F13             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F14             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F15             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F16             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F17             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F18             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F19             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F20             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F21             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F22             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F23             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F24             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_F25             , EventKeyboard::KeyCode::KEY_NONE          },
    { GLFW_KEY_KP_0            , EventKeyboard::KeyCode::KEY_0             },
    { GLFW_KEY_KP_1            , EventKeyboard::KeyCode::KEY_1             },
    { GLFW_KEY_KP_2            , EventKeyboard::KeyCode::KEY_2             },
    { GLFW_KEY_KP_3            , EventKeyboard::KeyCode::KEY_3             },
    { GLFW_KEY_KP_4            , EventKeyboard::KeyCode::KEY_4             },
    { GLFW_KEY_KP_5            , EventKeyboard::KeyCode::KEY_5             },
    { GLFW_KEY_KP_6            , EventKeyboard::KeyCode::KEY_6             },
    { GLFW_KEY_KP_7            , EventKeyboard::KeyCode::KEY_7             },
    { GLFW_KEY_KP_8            , EventKeyboard::KeyCode::KEY_8             },
    { GLFW_KEY_KP_9            , EventKeyboard::KeyCode::KEY_9             },
    { GLFW_KEY_KP_DECIMAL      , EventKeyboard::KeyCode::KEY_PERIOD        },
    { GLFW_KEY_KP_DIVIDE       , EventKeyboard::KeyCode::KEY_KP_DIVIDE     },
    { GLFW_KEY_KP_MULTIPLY     , EventKeyboard::KeyCode::KEY_KP_MULTIPLY   },
    { GLFW_KEY_KP_SUBTRACT     , EventKeyboard::KeyCode::KEY_KP_MINUS      },
    { GLFW_KEY_KP_ADD          , EventKeyboard::KeyCode::KEY_KP_PLUS       },
    { GLFW_KEY_KP_ENTER        , EventKeyboard::KeyCode::KEY_KP_ENTER      },
    { GLFW_KEY_KP_EQUAL        , EventKeyboard::KeyCode::KEY_EQUAL         },
    { GLFW_KEY_LEFT_SHIFT      , EventKeyboard::KeyCode::KEY_SHIFT         },
    { GLFW_KEY_LEFT_CONTROL    , EventKeyboard::KeyCode::KEY_CTRL          },
    { GLFW_KEY_LEFT_ALT        , EventKeyboard::KeyCode::KEY_ALT           },
    { GLFW_KEY_LEFT_SUPER      , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_RIGHT_SHIFT     , EventKeyboard::KeyCode::KEY_SHIFT         },
    { GLFW_KEY_RIGHT_CONTROL   , EventKeyboard::KeyCode::KEY_CTRL          },
    { GLFW_KEY_RIGHT_ALT       , EventKeyboard::KeyCode::KEY_ALT           },
    { GLFW_KEY_RIGHT_SUPER     , EventKeyboard::KeyCode::KEY_HYPER         },
    { GLFW_KEY_MENU            , EventKeyboard::KeyCode::KEY_MENU          },
    { GLFW_KEY_LAST            , EventKeyboard::KeyCode::KEY_NONE          }
};

//////////////////////////////////////////////////////////////////////////
// implement GLView
//////////////////////////////////////////////////////////////////////////


GLView::GLView()
: _captured(false)
, _supportTouch(false)
, _isInRetinaMonitor(false)
, _isRetinaEnabled(false)
, _retinaFactor(1)
, _frameZoomFactor(1.0f)
, _mainWindow(nullptr)
, _monitor(nullptr)
, _mouseX(0.0f)
, _mouseY(0.0f)
{
    _viewName = "cocos2dx";
    g_keyCodeMap.clear();
    for (auto& item : g_keyCodeStructArray)
    {
        g_keyCodeMap[item.glfwKeyCode] = item.keyCode;
    }

    GLFWEventHandler::setGLView(NULL);

    //glfwSetErrorCallback(GLFWEventHandler::onGLFWError);
    //glfwInit();
}

GLView::~GLView()
{
    // CCLOGINFO("deallocing GLView: %p", this);
    // GLFWEventHandler::setGLView(nullptr);
    // glfwTerminate();
}

GLView* GLView::create(const std::string& viewName)
{
    auto ret = new GLView;
    if(ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

GLView* GLView::createWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{
    auto ret = new GLView;
    if(ret && ret->initWithRect(viewName, rect, frameZoomFactor)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

GLView* GLView::createWithFullScreen(const std::string& viewName)
{
    auto ret = new GLView();
    if(ret && ret->initWithFullScreen(viewName)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

GLView* GLView::createWithFullScreen(const std::string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    auto ret = new GLView();
    if(ret && ret->initWithFullscreen(viewName, videoMode, monitor)) {
        ret->autorelease();
        return ret;
    }
    
    return nullptr;
}


bool GLView::initWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{

    return true;
}

bool GLView::initWithFullScreen(const std::string& viewName)
{
    return true;
}

bool GLView::initWithFullscreen(const std::string &viewname, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
    return true;
}

bool GLView::isOpenGLReady()
{
    return true;
}

void GLView::end()
{
    // Release self. Otherwise, GLView could not be freed.
    release();
}

void GLView::swapBuffers()
{
   
}

bool GLView::windowShouldClose()
{
    return false;
}

void GLView::pollEvents()
{
    
}


void GLView::enableRetina(bool enabled)
{

}


void GLView::setIMEKeyboardState(bool /*bOpen*/)
{

}

void GLView::setFrameZoomFactor(float zoomFactor)
{
   
}

float GLView::getFrameZoomFactor()
{
    return 1.f;
}

void GLView::updateFrameSize()
{
   
}

void GLView::setFrameSize(float width, float height)
{
   
}

void GLView::setViewPortInPoints(float x , float y , float w , float h)
{
  
}

void GLView::setScissorInPoints(float x , float y , float w , float h)
{
   
}

void GLView::onGLFWError(int errorID, const char* errorDesc)
{
   
}

void GLView::onGLFWMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
   
}

void GLView::onGLFWMouseMoveCallBack(GLFWwindow* window, double x, double y)
{
   
}

void GLView::onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y)
{
  
}

void GLView::onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
   
}

void GLView::onGLFWCharCallback(GLFWwindow *window, unsigned int character)
{
   
}

void GLView::onGLFWWindowPosCallback(GLFWwindow *windows, int x, int y)
{
   
}

void GLView::onGLFWframebuffersize(GLFWwindow* window, int w, int h)
{
   
}

void GLView::onGLFWWindowSizeFunCallback(GLFWwindow *window, int width, int height)
{
   
}

bool GLView::initGlew()
{
    return true;
}

NS_CC_END // end of namespace cocos2d;
