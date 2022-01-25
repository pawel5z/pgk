// ==========================================================================
// AGL3 example usage
//===========================================================================
#include "AGL3Window.hpp"
#include "Camera.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    Camera *cam = nullptr;

    MyWin() = default;
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, fullscr, vers) {};
    void KeyCB(int key, int scancode, int action, int mods) override;
    void MainLoop() override;
    void ScrollCB(double xp, double yp) override;
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

void MyWin::ScrollCB(double xp, double yp) {
    AGLWindow::ScrollCB(xp, yp);
    if (yp > 0.)
        cam->pos *= .75;
    else if (yp < 0.)
        cam->pos *= 1.25;
}

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    glm::vec3 origin(0.f, 0.f, 0.f);
    Camera cam;
    this->cam = &cam;
    cam.pos = {0.f, 0.f, 1.f};
    cam.rot = glm::quatLookAtLH(origin - cam.pos, Transform::UP);
    cam.setFovY(60);
    cam.setNf({0.01f, 10.0f});

    float angSpeed = .001f;
    const float baseSpeed = .01f;
    float speed = baseSpeed;

    float mouseSens = 1.f;
    double refMouseXPos, refMouseYPos;

    glClearColor(.7f, .7f, .7f, .0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        glfwPollEvents();
        if (glfwGetKey(win(), GLFW_KEY_LEFT_SHIFT))
            speed /= 5.f;
        // TODO reset pos/rot button
        if (glfwGetKey(win(), GLFW_KEY_R) == GLFW_PRESS) {
            origin = {0.f, 0.f, 0.f};
            cam.pos = {0.f, 0.f, 1.f};
            cam.rot = glm::quatLookAtLH(origin - cam.pos, Transform::UP);
        }
        if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            double xPos, yPos;
            glfwGetCursorPos(win(), &xPos, &yPos);
            // pan left/right
            cam.pos -= cam.right() * (float)(xPos - refMouseXPos) * mouseSens * speed;
            origin -= cam.right() * (float)(xPos - refMouseXPos) * mouseSens * speed;
            // pan up/down
            cam.pos += cam.up() * (float)(yPos - refMouseYPos) * mouseSens * speed;
            origin += cam.up() * (float)(yPos - refMouseYPos) * mouseSens * speed;
        }
        if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double xPos, yPos;
            glfwGetCursorPos(win(), &xPos, &yPos);
            cam.rotateAround(origin, Transform::UP, (float)(xPos - refMouseXPos) * mouseSens * angSpeed);
            cam.rotateAround(origin, Transform::RIGHT, (float)(yPos - refMouseYPos) * mouseSens * angSpeed);
        }
        cam.rot = glm::quatLookAtLH(origin - cam.pos, Transform::UP);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        AGLErrors("main-afterdraw");
        glfwGetCursorPos(win(), &refMouseXPos, &refMouseYPos);
        cam.setAspect(aspect);
        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers
    } while (!(glfwGetKey(win(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
               glfwWindowShouldClose(win()) == 1));
    this->cam = nullptr;
}

int main(int argc, char *argv[]) {
    MyWin win;
    win.Init(1024, 768, "Assignment 7", 0, 33);
    win.MainLoop();
    return 0;
}
