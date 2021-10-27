// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <cstdlib>
#include <cstdio>
#include <glm/glm.hpp>

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"

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

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        // TODO draw scene elements
        AGLErrors("main-afterdraw");

        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        //glfwWaitEvents();

        // polygon filling
        if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // player moving
        } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            // TODO
        } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            // TODO
        } else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            // TODO
        } else if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            // TODO
        }
    } while(glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char *argv[]) {
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
