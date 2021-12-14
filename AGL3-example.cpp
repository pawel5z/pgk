// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include "AGL3Window.hpp"
#include "Camera.hpp"

#include <cstdlib>
#include <cstdio>
#include <memory>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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

}

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    Camera cam;
    cam.setFovY(60);
    cam.setNf({0.01f, 100.0f});

    float angSpeed = .1f;

    double refMouseXPos, refMouseYPos;

    glClearColor(.8f, .8f, .8f, .0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        glfwPollEvents();
        //glfwWaitEvents();

        // mouse input
        if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            float mouseSens = .1;
            double xPos, yPos;
            glfwGetCursorPos(win(), &xPos, &yPos);
            // yaw
            cam.rotate(Transform::UP, -angSpeed * mouseSens * (float) (xPos - refMouseXPos), SELF);
            // pitch
            cam.rotate(Transform::RIGHT, angSpeed * mouseSens * (float) (yPos - refMouseYPos), SELF);
        }
        glfwGetCursorPos(win(), &refMouseXPos, &refMouseYPos);

        cam.setAspect(aspect);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers
    } while (!(glfwGetKey(win(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
               glfwWindowShouldClose(win()) == 1));
}

int main(int argc, char *argv[]) {
    MyWin win;
    win.Init(800,600,"Assignment 6",0,33);
    win.MainLoop();
    return 0;
}
