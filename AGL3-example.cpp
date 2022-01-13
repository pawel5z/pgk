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
#include "Sphere.hpp"
#include "sharedState.hpp"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>

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

size_t drawnTrianglesCnt = 0;

// display performance statistics once per `interval` seconds
double displayPerformance(double interval = 1.f) {
    static double lastTimestamp = 0.;
    static double timeElapsed = 0.;
    static size_t numberOfCalls = 0;

    double currTimestamp = glfwGetTime();
    numberOfCalls++;
    timeElapsed += currTimestamp - lastTimestamp;
    if (timeElapsed >= interval) {
        fprintf(stderr,
                "\rfps: %lf, ms per frame: %lf, triangles per frame: %lu                           ",
                (double)numberOfCalls / timeElapsed,
                timeElapsed * 1000. / (double)numberOfCalls,
                drawnTrianglesCnt);
        numberOfCalls = 0;
        timeElapsed = 0.;
    }
    lastTimestamp = currTimestamp;
    drawnTrianglesCnt = 0;
    return (double)numberOfCalls / timeElapsed;
}

void lodAutoControl(Terrain &terrain) {
    static double lastTimestamp = 0.;

    double currentTimestamp = glfwGetTime();
    if (currentTimestamp - lastTimestamp < 2.f) // try modifying LOD every 2 seconds
        return;
    lastTimestamp = currentTimestamp;
    GLubyte lod = terrain.getLod();
    double fps = displayPerformance();
    try {
        if (fps <= 10.) {
            terrain.setLod(lod + 1);
            printf("\nLOD set to %i.\n", terrain.getLod() + 1);
        } else if (fps > 30.) {
            terrain.setLod(lod - 1);
            printf("\nLOD set to %i.\n", terrain.getLod() + 1);
        }
    } catch (std::exception &e) {
        return;
    }
}

static std::string dirPath;
static const float earthRadius = 6371.008f;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    bool drawMapMode = true;

    Sphere earth(36, 72);
    earth.scale = Transform::ONE * 2.f * earthRadius;

    Terrain terrain(dirPath);
    terrain.setLod(2);
    bool autoLod = true;

    Camera cam;
    cam.pos = {terrain.getMidLo(), terrain.getMidLa(), 1.f};
    cam.rot = glm::quatLookAtLH(glm::normalize(glm::vec3(terrain.getMidLo(), terrain.getMidLa(), 0.f) - cam.pos), Transform::UP);
    cam.setFovY(60);
    cam.setNf({0.01f, 100.0f});

    float angSpeed = .001f;
    const float baseSpeed = 0.025f;
    float speed = baseSpeed;

    double refMouseXPos, refMouseYPos;
    int lastTabState = GLFW_RELEASE;

    glLineWidth(2.5f);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(primitiveRestartIndex);
    glClearColor(.7f, .7f, .7f, .0f);
    // enable depth buffer comparisons
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    do {
        if (autoLod)
            lodAutoControl(terrain);
        (void)displayPerformance(0.03125);
        glfwPollEvents();

        if (drawMapMode) {
            speed = baseSpeed * cam.pos.z;
            if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
                cam.pos += speed * cam.forward();
            if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
                cam.pos -= speed * cam.forward();
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            AGLErrors("main-loopbegin");
            // =====================================================        Drawing
            terrain.draw(cam, false);
            AGLErrors("main-afterdraw");

            int currentTabState = glfwGetKey(win(), GLFW_KEY_TAB);
            if (currentTabState == GLFW_PRESS && lastTabState == GLFW_RELEASE) {
                // switch to 3d mode
                drawMapMode = !drawMapMode;
                cam.pos = pointOnSphere(cam.pos.y, cam.pos.x, earthRadius + 10.f);
                cam.rot = glm::quatLookAtLH(glm::normalize(Transform::ZERO - cam.pos), Transform::UP);
                cam.setNf({.1f, 20000.f});
            }
            lastTabState = currentTabState;
        } else {
            float distFromZero = glm::length(cam.pos);
            speed = .5f;
            if (glfwGetKey(win(), GLFW_KEY_LEFT_SHIFT))
                speed /= 5.f;
            // go up
            if (glfwGetKey(win(), GLFW_KEY_E) == GLFW_PRESS)
                cam.pos += glm::normalize(cam.pos) * speed;
            // go down
            if (glfwGetKey(win(), GLFW_KEY_Q) == GLFW_PRESS)
                cam.pos -= glm::normalize(cam.pos) * speed;
            if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
                cam.pos = glm::normalize(cam.pos + cam.forward() * speed) * distFromZero;
            if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
                cam.pos = glm::normalize(cam.pos - cam.forward() * speed) * distFromZero;
            if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
                cam.pos = glm::normalize(cam.pos + cam.right() * speed) * distFromZero;
            if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
                cam.pos = glm::normalize(cam.pos - cam.right() * speed) * distFromZero;
            if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                float mouseSens = 1.f;
                double xPos, yPos;
                glfwGetCursorPos(win(), &xPos, &yPos);
                // yaw
                cam.rotate(-glm::normalize(cam.pos), (float)(xPos - refMouseXPos) * mouseSens * angSpeed, WORLD);
                // pitch
                cam.rotate(-cam.right(), (float)(yPos - refMouseYPos) * mouseSens * angSpeed, WORLD);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            AGLErrors("main-loopbegin");
            // =====================================================        Drawing
            terrain.draw(cam, true);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            earth.draw(cam);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            AGLErrors("main-afterdraw");

            int currentTabState = glfwGetKey(win(), GLFW_KEY_TAB);
            if (currentTabState == GLFW_PRESS && lastTabState == GLFW_RELEASE) {
                // switch to map mode
                drawMapMode = !drawMapMode;
                double la = glm::degrees(glm::asin(cam.pos.y / glm::length(cam.pos)));
                double lo = glm::degrees(atan2(cam.pos.x, cam.pos.z) + M_PI);
                cam.pos = {lo, la, 1.f};
                cam.rot = glm::quatLookAtLH(glm::normalize(glm::vec3(lo, la, 0.f) - cam.pos), Transform::UP);
                cam.setNf({0.01f, 100.0f});
            }
            lastTabState = currentTabState;
        }
        glfwGetCursorPos(win(), &refMouseXPos, &refMouseYPos);

        // LOD controls
        if (glfwGetKey(win(), GLFW_KEY_0) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_KP_0) == GLFW_PRESS) {
            autoLod = true;
        } else if (glfwGetKey(win(), GLFW_KEY_1) == GLFW_PRESS ||
            glfwGetKey(win(), GLFW_KEY_KP_1) == GLFW_PRESS) {
            autoLod = false;
            terrain.setLod(0);
        } else if (glfwGetKey(win(), GLFW_KEY_2) == GLFW_PRESS ||
                   glfwGetKey(win(), GLFW_KEY_KP_2) == GLFW_PRESS) {
            autoLod = false;
            terrain.setLod(1);
        } else if (glfwGetKey(win(), GLFW_KEY_3) == GLFW_PRESS ||
                   glfwGetKey(win(), GLFW_KEY_KP_3) == GLFW_PRESS) {
            autoLod = false;
            terrain.setLod(2);
        } else if (glfwGetKey(win(), GLFW_KEY_4) == GLFW_PRESS ||
                   glfwGetKey(win(), GLFW_KEY_KP_4) == GLFW_PRESS) {
            autoLod = false;
            terrain.setLod(3);
        } else if (glfwGetKey(win(), GLFW_KEY_5) == GLFW_PRESS ||
                   glfwGetKey(win(), GLFW_KEY_KP_5) == GLFW_PRESS) {
            autoLod = false;
            terrain.setLod(4);
        }

        cam.setAspect(aspect);
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
    win.Init(1024,768,"Assignment 6",0,33);
    win.MainLoop();
    return 0;
}
