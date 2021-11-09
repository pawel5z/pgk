// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <sstream>
#include <getopt.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "AGL3Window.hpp"

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() = default;
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, fullscr, vers) {};
    void KeyCB(int key, int scancode, int action, int mods) override;
    void MainLoop() override;
};

// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key,scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
       ; // do something
    }
    if (key == GLFW_KEY_HOME  && (action == GLFW_PRESS)) {
       ; // do something
    }
}

static int latticeSize = 10;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    int player = 0;
    GLfloat speed = 0.005;
    GLfloat angSpeed = 0.02;

    bool gameOver = false;
    double startTime = glfwGetTime();

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers

        // game over check

        glfwPollEvents();
        //glfwWaitEvents();

        // player movement
    } while(glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 &&
            !gameOver);
}

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int c;
    static struct option long_options[] = {
            {"seed", required_argument, nullptr, 's'},
            {nullptr, 0, nullptr, 0}
    };
    int conv;
    std::stringstream ss;
    while ((c = getopt_long(argc, argv, "s:n:", long_options, nullptr)) != -1) {
        ss.clear();
        switch (c) {
            case 's':
                ss << optarg;
                ss >> conv;
                if (!ss.fail())
                    srand(conv);
                break;
            case 'n':
                ss << optarg;
                ss >> conv;
                if (!ss.fail() && conv >= 2)
                    latticeSize = conv;
                break;
            default:
                break;
        }
    }
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
