// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include "AGL3Window.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "utils.hpp"

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

// display performance statistics once per `interval` seconds
void displayPerformance(double interval = 1.f) {
    static double lastTimestamp = 0.;
    static double timeElapsed = 0.;
    static size_t numberOfCalls = 0;

    double currTimestamp = glfwGetTime();
    numberOfCalls++;
    timeElapsed += currTimestamp - lastTimestamp;
    if (timeElapsed >= interval) {
        fprintf(stderr,
                "\rfps: %lf, ms per frame: %lf"
                "                                                                                ",
                (double)numberOfCalls / timeElapsed,
                timeElapsed * 1000. / (double)numberOfCalls);
        numberOfCalls = 0;
        timeElapsed = 0.;
    }
    lastTimestamp = currTimestamp;
}

static std::string dirPath;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    Terrain terrain(dirPath);

    Camera cam;
    cam.pos = {terrain.getMidLo(), terrain.getMidLa(), 1.f};
    cam.rot = glm::quatLookAtLH(glm::vec3(terrain.getMidLo(), terrain.getMidLa(), 0.f) - cam.pos, Transform::UP);
    cam.setFovY(60);
    cam.setNf({0.01f, 100.0f});

    float angSpeed = .01f;
    float speed = .01f;

    double refMouseXPos, refMouseYPos;

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(primitiveRestartIndex);
    glClearColor(.7f, .7f, .7f, .0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        displayPerformance(0.03125);

        glfwPollEvents();
        //glfwWaitEvents();

        if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
            cam.pos += speed * cam.forward();
        if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
            cam.pos -= speed * cam.forward();
        // rightward
        if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
            cam.rotate(Transform::UP, angSpeed, Space::WORLD);
        // leftward
        if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
            cam.rotate(Transform::UP, -angSpeed, Space::WORLD);
        // mouse input
        if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            float mouseSens = .1;
            double xPos, yPos;
            glfwGetCursorPos(win(), &xPos, &yPos);
            // pan left/right
            cam.pos -= cam.right() * (float)(xPos - refMouseXPos) * mouseSens * speed;
            // pan up/down
            cam.pos += cam.up() * (float)(yPos - refMouseYPos) * mouseSens * speed;
        }
        glfwGetCursorPos(win(), &refMouseXPos, &refMouseYPos);

        cam.setAspect(aspect);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        terrain.draw(cam);
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers
    } while (!(glfwGetKey(win(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
               glfwWindowShouldClose(win()) == 1));
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "unspecified directory path");
        exit(EXIT_FAILURE);
    }
    dirPath = argv[1];
    MyWin win;
    win.Init(800,600,"Assignment 6",0,33);
    win.MainLoop();
    return 0;
}
