// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include "AGL3Window.hpp"
#include "Sphere.hpp"
#include "TetraGrid.hpp"
#include "Cube.hpp"

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

// TODO This is just some approximation. Make accurate collision detection.
bool doesCollide(const Sphere &s, const TetraGrid &t, int i) {
    float r = .5f * (s.scale.x + s.scale.y + s.scale.z) / 3.f;
    // collision detection with vertices
    for (auto &vertex : t.getVerticesPos(i))
        if (glm::distance2(s.pos, vertex) <= r * r)
            return true;
    return false;
}

bool doesCollide(const Sphere &s, const TetraGrid &t) {
    float r = .5f * (s.scale.x + s.scale.y + s.scale.z) / 3.f;
    for (int i = 0; i < t.size(); i++) {
        // rough filtering
        float checkThreshold = 2.f * r * glm::root_three<GLfloat>() / 2.f;
        if (glm::distance2(s.pos, t.getPos(i)) > checkThreshold * checkThreshold)
            continue;
        // exact check
        if (doesCollide(s, t, i))
            return true;
    }
    return false;
}

bool isSphereInsideBox(const Sphere &s, const Cube &c) {
    float r = .5f * (s.scale.x + s.scale.y + s.scale.z) / 3.f;
    return s.pos.x + r <= c.pos.x + c.scale.x * .5 &&
           s.pos.y + r <= c.pos.y + c.scale.y * .5 &&
           s.pos.z + r <= c.pos.z + c.scale.z * .5 &&
           s.pos.x - r >= c.pos.x - c.scale.x * .5 &&
           s.pos.y - r >= c.pos.y - c.scale.y * .5 &&
           s.pos.z - r >= c.pos.z - c.scale.z * .5;
}

static int latticeSize = 4;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);
    glfwSetInputMode(win(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    Sphere s(45);
    s.rot = glm::quatLookAt(glm::normalize(-Transform::ONE), Transform::UP);
    float r = 0.75f * .5f / (float)latticeSize;
    s.scale = Transform::ONE * .75f / (float)latticeSize;
    GLfloat speed = 0.01;
    GLfloat angSpeed = 0.015;

    Camera cam;
    cam.setFovY(60);
    cam.setNf({0.01f, 10.0f});

    float ortCamRange = 3;
    Camera camOrt(-ortCamRange * r, ortCamRange * r, -ortCamRange * r, ortCamRange * r, 0, 6.f * r);

    TetraGrid tetraGrid(latticeSize);

    Cube box;
    box.pos = Transform::ONE * .5f;
    // make sure the sphere at [0, 0, 0] fits in the box
    box.scale *= 1 + 2.f * 3.f / 8.f / (float)latticeSize;

    bool gameOver = false;

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        cam.pos = s.pos;
        cam.rot = s.rot;
        camOrt.rot = cam.rot;
        camOrt.pos = cam.pos - 2.0f * r * camOrt.forward();

        cam.setAspect(aspect);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        // >>> main camera
        ViewportOne(0, 0, wd, ht);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLineWidth(1.f);
        // don't draw sphere in FPP view
        tetraGrid.draw(cam);
        box.draw(cam, (float)glfwGetTime());
        // <<< main camera
        // >>> secondary camera
        int m = glm::min(wd, ht);
        glScissor(wd - m / 3.0f, 0, m / 3.0f, m / 3.0f);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Viewport(wd - m / 3.0f, 0, m / 3.0f, m / 3.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(3.f);
        s.draw(camOrt);
        tetraGrid.draw(camOrt);
        box.draw(camOrt, (float)glfwGetTime());
        glDisable(GL_SCISSOR_TEST);
        // <<< secondary camera
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers

        glfwPollEvents();
        //glfwWaitEvents();

        glm::vec3 oldPos = s.pos;
        // player movement
        // yaw
        if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            s.rotate(Transform::UP, -angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            s.rotate(Transform::UP, angSpeed, SELF);
        }
        // pitch
        if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            s.rotate(Transform::RIGHT, angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            s.rotate(Transform::RIGHT, -angSpeed, SELF);
        }
        // roll
        if (glfwGetKey(win(), GLFW_KEY_E) == GLFW_PRESS) {
            s.rotate(Transform::FORWARD, angSpeed, SELF);
        }
        if (glfwGetKey(win(), GLFW_KEY_Q) == GLFW_PRESS) {
            s.rotate(Transform::FORWARD, -angSpeed, SELF);
        }
        // move forwards/backwards
        if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS) {
            s.pos = s.pos + speed * s.forward();
        }
        if (glfwGetKey(win(), GLFW_KEY_Z) == GLFW_PRESS) {
            s.pos = s.pos - speed * s.forward();
        }
        float mouseSens = .1;
        double xPos, yPos;
        glfwGetCursorPos(win(), &xPos, &yPos);
        // yaw
        s.rotate(Transform::UP, -angSpeed * mouseSens * (xPos - wd / 2.), SELF);
        // pitch
        s.rotate(Transform::RIGHT, angSpeed * mouseSens * (yPos - ht / 2.), SELF);

        // game over check
        if (doesCollide(s, tetraGrid, tetraGrid.size() - 1)) {
            printf("You win!\n");
            break;
        }

        if (doesCollide(s, tetraGrid)
            // restrict movement by box boundary
            || !isSphereInsideBox(s, box)) {
            s.pos = oldPos;
        }

        // reset cursor position
        glfwSetCursorPos(win(), wd / 2, ht / 2);

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
