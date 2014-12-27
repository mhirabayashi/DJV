//------------------------------------------------------------------------------
// Copyright (c) 2004-2014 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djvGlxContextPrivate.cpp

#include <djvGlxContextPrivate.h>

#include <djvDebug.h>
#include <djvDebugLog.h>

#include <QCoreApplication>

//------------------------------------------------------------------------------
// djvGlxContextPrivate
//------------------------------------------------------------------------------

djvGlxContextPrivate::djvGlxContextPrivate() throw (djvError) :
    _display     (0),
    _screen      (0),
    _visuals     (0),
    _visualsCount(0),
    _colormap    (0),
    _window      (0),
    _context     (0)
{
    //DJV_DEBUG("djvGlxContextPrivate::djvGlxContextPrivate");

    // Open the X display.
    
    DJV_LOG("djvGlxContextPrivate", "Opening the X display...");

    _display = XOpenDisplay(NULL);

    if (! _display)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_X_DISPLAY]);
    }
    
    _screen = DefaultScreen(_display);

    DJV_LOG("djvGlxContextPrivate", QString("X screen: %1").arg(_screen));

    // Choose a visual.

    static const int depths [] =
    {
        32, 24
    };
    
    static const int depthsCount = sizeof(depths) / sizeof(depths[0]);

    XVisualInfo visualInfo;
    visualInfo.screen = _screen;
    
    for (int i = 0; i < depthsCount; ++i)
    {
        DJV_LOG("djvGlxContextPrivate",
            QString("Checking for a X visual with a depth of %1...").
            arg(depths[i]));

        visualInfo.depth = depths[i];

        _visuals = XGetVisualInfo(
            _display,
            VisualScreenMask | VisualDepthMask,
            &visualInfo,
            &_visualsCount);
        
        if (_visuals && _visualsCount)
            break;
    }
    
    //DJV_DEBUG_PRINT("depth = " << visualInfo.depth);
    
    if (! _visuals || ! _visualsCount)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_X_VISUALS]);
    }

    // Create the color map.
	
	DJV_LOG("djvGlxContextPrivate", "Creating the color map...");

    _colormap = XCreateColormap(
        _display,
        RootWindow(_display, _screen),
        _visuals[0].visual,
        AllocNone);

    if (! _colormap)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_X_COLORMAP]);
    }

    // Check for GLX support.

	DJV_LOG("djvGlxContextPrivate", "Checking for GLX...");
	
    if (! glXQueryExtension(_display, 0, 0))
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_GLX]);
    }

    // Create a dummy window and OpenGL context for glewInit.

	DJV_LOG("djvGlxContextPrivate", "Creating dummy window...");

    XSetWindowAttributes winAttrib;
    winAttrib.colormap     = _colormap;
    winAttrib.border_pixel = 0;

    _window = XCreateWindow(
        _display,
        RootWindow(_display, _screen),
        0, 0, 1, 1, 0,
        visualInfo.depth,
        InputOutput,
        _visuals[0].visual,
        CWColormap | CWBorderPixel,
        &winAttrib);

    if (! _window)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_X_WINDOW]);
    }

    // Create the OpenGL context.

	DJV_LOG("djvGlxContextPrivate", "Creating OpenGL context...");

    _context = glXCreateContext(
        _display,
        &_visuals[0],
        0,
        True);

    if (! _context)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_CREATE_CONTEXT]);
    }

    // Bind the context.

    bind();

    // Initialize GLEW.
	
    DJV_LOG("djvGlxContextPrivate", "Initializing GLEW...");

    GLint glError = glewInit();

    if (glError != GLEW_OK)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_INIT_GLEW].arg(glError));
    }

    setVendor((const char *)glGetString(GL_VENDOR));
    setRenderer((const char *)glGetString(GL_RENDERER));
    setVersion((const char *)glGetString(GL_VERSION));

    //DJV_DEBUG_PRINT("vendor string = " << vendor());
    //DJV_DEBUG_PRINT("renderer string = " << renderer());
    //DJV_DEBUG_PRINT("version string = " << version());
    //DJV_DEBUG_PRINT("extensions = " <<
    //    (const char *)glGetString(GL_EXTENSIONS));
    //DJV_DEBUG_PRINT("glu version = " <<
    //    (const char *)gluGetString(GLU_VERSION));
    //DJV_DEBUG_PRINT("glu extensions = " <<
    //    (const char *)gluGetString(GLU_EXTENSIONS));

    DJV_LOG("djvGlxContextPrivate", QString("GL vendor: \"%1\"").arg(vendor()));
    DJV_LOG("djvGlxContextPrivate", QString("GL renderer: \"%1\"").arg(renderer()));
    DJV_LOG("djvGlxContextPrivate", QString("GL version: \"%1\"").arg(version()));

    if (! GL_EXT_framebuffer_object)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_NO_FBO]);
    }
}

djvGlxContextPrivate::~djvGlxContextPrivate()
{
    //DJV_DEBUG("djvGlxContextPrivate::~djvGlxContextPrivate");

    if (_context)
    {
        //DJV_DEBUG_PRINT("context");

        glXDestroyContext(_display, _context);
    }

    if (_window)
    {
        //DJV_DEBUG_PRINT("window");

        XDestroyWindow(_display, _window);
    }

    if (_colormap)
    {
        XFreeColormap(_display, _colormap);
    }

    if (_visuals)
    {
        XFree(_visuals);
    }

    if (_display)
    {
        //DJV_DEBUG_PRINT("display");

        XCloseDisplay(_display);
    }
}

const QStringList & djvGlxContextPrivate::errorLabels()
{
    static const QStringList data = QStringList() <<
        qApp->translate("djvGlxContextPrivate", "Cannot open X display") <<
        qApp->translate("djvGlxContextPrivate", "No appropriate X visuals") <<
        qApp->translate("djvGlxContextPrivate", "Cannot create X colormap") <<
        qApp->translate("djvGlxContextPrivate", "No GLX extension") <<
        qApp->translate("djvGlxContextPrivate", "Cannot create an X window") <<
        qApp->translate("djvGlxContextPrivate", "Cannot create OpenGL context") <<
        qApp->translate("djvGlxContextPrivate", "Cannot initialize GLEW: #%1") <<
        qApp->translate("djvGlxContextPrivate", "No OpenGL FBO support") <<
        qApp->translate("djvGlxContextPrivate", "Invalid OpenGL context") <<
        qApp->translate("djvGlxContextPrivate", "Cannot bind OpenGL context");
    
    DJV_ASSERT(ERROR_COUNT == data.count());
    
    return data;
}

void djvGlxContextPrivate::bind() throw (djvError)
{
    if (! _context)
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_INVALID_CONTEXT]);
    }

    //DJV_DEBUG("djvGlxContextPrivate::bind");

    if (! glXMakeCurrent(_display, _window, _context))
    {
        throw djvError(
            "djvGlxContextPrivate",
            errorLabels()[ERROR_BIND_CONTEXT]);
    }
}

void djvGlxContextPrivate::unbind()
{
    //DJV_DEBUG("djvGlxContextPrivate::unbind");

    glXMakeCurrent(_display, 0, 0);
}

