// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include "AGL3Window.hpp"
#include "Sphere.hpp"
#include "Cube.hpp"
#include "BubbleContainer.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"

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
    Camera *cam = nullptr;

    MyWin() = default;
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, fullscr, vers) {};
    void KeyCB(int key, int scancode, int action, int mods) override;
    void MainLoop() override;
    void ScrollCB(double xp, double yp) override;
};

void MyWin::ScrollCB(double xp, double yp) {
    AGLWindow::ScrollCB(xp, yp);
    float fov = cam->getFovY();
    cam->setFovY(fov + (float)(yp < .0 && fov < 90.f) * 1.f  - (float)(yp > .0 && fov > 45.f) * 1.f);
}

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

bool isSphereInsideBox(const Sphere &s, const Cube &c) {
    float r = .5f * (s.scale.x + s.scale.y + s.scale.z) / 3.f;
    return s.pos.x + r <= c.pos.x + c.scale.x * .5 &&
           s.pos.y + r <= c.pos.y + c.scale.y * .5 &&
           s.pos.z + r <= c.pos.z + c.scale.z * .5 &&
           s.pos.x - r >= c.pos.x - c.scale.x * .5 &&
           s.pos.y - r >= c.pos.y - c.scale.y * .5 &&
           s.pos.z - r >= c.pos.z - c.scale.z * .5;
}

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);
    int level = 1;
    bool gameOver = false;
    double lastFrameTimeStamp = glfwGetTime();

    Camera cam;
    cam.setFovY(60);
    cam.setNf({0.01f, 10.0f});

    Camera outCam;
    outCam.setFovY(60);
    outCam.setNf({.01f, 10.f});
    outCam.pos = {2.f, 2.f, 2.5f};
    outCam.rot = glm::quatLookAtLH(glm::normalize(glm::vec3(-1.f, -1.f, 0.f)), Transform::UP);

    while (!gameOver) {
        Sphere player(20);
        player.rot = glm::quatLookAtLH(Transform::FORWARD, Transform::UP);
        float r = .1f;
        player.scale = Transform::ONE * 2.f * r;
        player.pos = player.forward() * r + 1e-6f;
        GLfloat speed = .05f;
        GLfloat angSpeed = 0.015;

        this->cam = &cam;
        Camera *drawingCam = &cam;

        Cube box;
        box.scale.z *= 5.f;
        box.pos.z = .5f * box.scale.z;

        float bubbleRadius = .1f;
        float targetMul = 1.5f;
        BubbleContainer bc(20, 1000, 1 + (int)glm::log2((float)level), .005f, -.5f + bubbleRadius, .5f - bubbleRadius * targetMul, -.2f, .2f, 2.f * r + 2.f * bubbleRadius * 1.5f, 4.5f, 2.f * bubbleRadius, 2.f * bubbleRadius * targetMul);

        DirectionalLight directionalLight(Transform::DOWN, {1.f, 1.f, 1.f});
        directionalLight.setIntensity(1.f);

        PointLight pl(player.pos, {.878f, 0.f, .901f});
        pl.setIntensity(2.f);

        double refMouseXPos, refMouseYPos;

        int tabPrevState = GLFW_RELEASE;

        glClearColor(.8f, .8f, .8f, .0f);
        // enable depth buffer comparisons
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // set up blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        while (!gameOver) {
            double currentFrameTimeStamp = glfwGetTime();

            glfwPollEvents();
            //glfwWaitEvents();

            // camera toggle
            int tabState = glfwGetKey(win(), GLFW_KEY_TAB);
            if (tabState == GLFW_PRESS && tabPrevState == GLFW_RELEASE)
                drawingCam = drawingCam == &cam ? &outCam : &cam;
            tabPrevState = tabState;

            glm::vec3 oldPos = player.pos;
            // player movement
            // roll
            if (glfwGetKey(win(), GLFW_KEY_E) == GLFW_PRESS)
                player.rotate(Transform::FORWARD, angSpeed, SELF);
            if (glfwGetKey(win(), GLFW_KEY_Q) == GLFW_PRESS)
                player.rotate(Transform::FORWARD, -angSpeed, SELF);
            // forward
            if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
                player.pos += speed * player.forward();
            // backward
            if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
                player.pos -= speed * player.forward();
            // rightward
            if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
                player.pos += speed * player.right();
            // leftward
            if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS ||
                glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
                player.pos -= speed * player.right();
            // up
            if (glfwGetKey(win(), GLFW_KEY_PAGE_UP) == GLFW_PRESS)
                player.pos += speed * player.up();
            // down
            if (glfwGetKey(win(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
                player.pos -= speed * player.up();
            // mouse input
            if (glfwGetMouseButton(win(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                float mouseSens = .1;
                double xPos, yPos;
                glfwGetCursorPos(win(), &xPos, &yPos);
                // yaw
                player.rotate(Transform::UP, -angSpeed * mouseSens * (float) (xPos - refMouseXPos), SELF);
                // pitch
                player.rotate(Transform::RIGHT, angSpeed * mouseSens * (float) (yPos - refMouseYPos), SELF);
            }
            glfwGetCursorPos(win(), &refMouseXPos, &refMouseYPos);

            if (// restrict movement by box boundary
                    !isSphereInsideBox(player, box)) {
                player.pos = oldPos;
            }

            cam.rot = player.rot;
            // camera TPP
            cam.pos = player.pos - 3 * r * cam.forward() + r * cam.up();
            pl.pos = player.pos;
            cam.setAspect(aspect);
            outCam.setAspect(aspect);

            bc.update((float)(currentFrameTimeStamp - lastFrameTimeStamp));

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            AGLErrors("main-loopbegin");
            // =====================================================        Drawing
            box.draw(*drawingCam, directionalLight, pl);
            player.draw(*drawingCam, directionalLight);
            bc.draw(*drawingCam, directionalLight, pl);
            AGLErrors("main-afterdraw");

            WaitForFixedFPS();
            glfwSwapBuffers(win()); // =============================   Swap buffers

            // check level complete
            if (player.pos.z >= 5.f - 1.5f * r) {
                printf("Level %d complete!\n", level++);
                break;
            }

            // check game over
            if (bc.doesCollide(player)) {
                printf("Game over at level %d.\n", level);
                gameOver = true;
            }

            // check for close request
            if (glfwGetKey(win(), GLFW_KEY_ESCAPE) == GLFW_PRESS ||
                glfwWindowShouldClose(win()) == 1)
                gameOver = true;

            lastFrameTimeStamp = currentFrameTimeStamp;
        }
        this->cam = nullptr;
    }
}

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
