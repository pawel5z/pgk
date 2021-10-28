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
    std::stringstream ss;
    if (argc > 1) {
        ss << argv[1];
        unsigned int seed;
        ss >> seed;
        if (!ss.fail())
            srand(seed);
        else
            ss.clear();
    }
    if (argc > 2) {
        ss << argv[2];
        int newLatticeSize;
        ss >> newLatticeSize;
        if (!ss.fail())
            latticeSize = newLatticeSize;
    }
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
