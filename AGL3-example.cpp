// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include "AGL3Window.hpp"
#include "Sphere.hpp"

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

static int latticeSize = 4;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    Sphere s(45);
    float r = .9f * (float)latticeSize;
    s.scale = Transform::ONE * r;

    Camera cam;
    cam.pos = glm::vec3(0, 0, 0);
    cam.rot = glm::quatLookAt(glm::normalize(Transform::ONE), Transform::UP);

    int ortCamRange = 3;
    Camera camOrt(-ortCamRange * r, ortCamRange * r, -ortCamRange * r, ortCamRange * r, 0, 10);

    GLfloat speed = 0.1;
    GLfloat angSpeed = 0.02;

    bool gameOver = false;

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        s.pos = cam.pos;
        s.rot = cam.rot;
        camOrt.rot = cam.rot;
        camOrt.pos = cam.pos - 2.0f * r * camOrt.forward();

        cam.setAspect(aspect);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        // main camera
        ViewportOne(0, 0, wd, ht);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // don't draw sphere in FPP view
        // TODO draw obstacles
        // secondary camera
        int m = glm::min(wd, ht);
        Viewport(wd - m / 3.0f, 0, m / 3.0f, m / 3.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        s.draw(camOrt);
        // TODO draw obstacles
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers

        // game over check

        glfwPollEvents();
        //glfwWaitEvents();

        // player movement
        // yaw
        if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            cam.rotate(Transform::UP, -angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            cam.rotate(Transform::UP, angSpeed, SELF);
        }
        // pitch
        if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            cam.rotate(Transform::RIGHT, angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            cam.rotate(Transform::RIGHT, -angSpeed, SELF);
        }
        // roll
        if (glfwGetKey(win(), GLFW_KEY_E) == GLFW_PRESS) {
            cam.rotate(Transform::FORWARD, angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_Q) == GLFW_PRESS) {
            cam.rotate(Transform::FORWARD, -angSpeed, SELF);
        }
        // move forwards/backwards
        if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS) {
            cam.pos = cam.pos + speed * cam.forward();
        }
        if (glfwGetKey(win(), GLFW_KEY_Z) == GLFW_PRESS) {
            cam.pos = cam.pos - speed * cam.forward();
        }
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
    }
    ss.str("");
    ss.clear();
    if (argc > 2) {
        ss << argv[2];
        int newLatticeSize;
        ss >> newLatticeSize;
        if (!ss.fail() && newLatticeSize >= 2)
            latticeSize = newLatticeSize;
    }
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
