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
#include "Background.hpp"
#include "TriangleGrid.hpp"

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
    void quitLoop(TriangleGrid &tg, Background &bg, double quitTime = 3.0);
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

inline bool sameSide(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &p1, const glm::vec2 &p2) {
    glm::vec3 baDiff = glm::vec3(b - a, 0.0f);
    return glm::dot(glm::cross(baDiff, glm::vec3(p1 - a, 0.0f)),
                    glm::cross(baDiff, glm::vec3(p2 - a, 0.0f))) >= 0.0f;
}

bool isCollision(TriangleGrid &tg, int t1, int t2) {
    bool flag = true;
    // relative to edges of t1
    for (int i = 0; i < 3; i++) {
        int a = i, b = (i + 1) % 3, c = (i + 2) % 3;
        if (!sameSide(tg.getTriVertexWorldCoords(t1, a), tg.getTriVertexWorldCoords(t1, b), tg.getTriVertexWorldCoords(t1, c), tg.getTriVertexWorldCoords(t2, 0)) &&
            !sameSide(tg.getTriVertexWorldCoords(t1, a), tg.getTriVertexWorldCoords(t1, b), tg.getTriVertexWorldCoords(t1, c), tg.getTriVertexWorldCoords(t2, 1)) &&
            !sameSide(tg.getTriVertexWorldCoords(t1, a), tg.getTriVertexWorldCoords(t1, b), tg.getTriVertexWorldCoords(t1, c), tg.getTriVertexWorldCoords(t2, 2)))
            flag = false;
    }
    // relative to edges of t2
    for (int i = 0; i < 3; i++) {
        int a = i, b = (i + 1) % 3, c = (i + 2) % 3;
        if (!sameSide(tg.getTriVertexWorldCoords(t2, a), tg.getTriVertexWorldCoords(t2, b), tg.getTriVertexWorldCoords(t2, c), tg.getTriVertexWorldCoords(t1, 0)) &&
            !sameSide(tg.getTriVertexWorldCoords(t2, a), tg.getTriVertexWorldCoords(t2, b), tg.getTriVertexWorldCoords(t2, c), tg.getTriVertexWorldCoords(t1, 1)) &&
            !sameSide(tg.getTriVertexWorldCoords(t2, a), tg.getTriVertexWorldCoords(t2, b), tg.getTriVertexWorldCoords(t2, c), tg.getTriVertexWorldCoords(t1, 2)))
            flag = false;
    }
    return flag;
}

static int latticeSize = 10;

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    TriangleGrid tg(latticeSize, 1.0f / 12.0f);
    int player = 0;
    GLfloat speed = 0.005;
    GLfloat angSpeed = 0.02;
    Background bg;

    bool gameOver = false;
    double startTime = glfwGetTime();
    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        bg.draw(aspect);
        tg.draw(aspect);
        AGLErrors("main-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers

        for (int i = 1; i < latticeSize * latticeSize - 1; i++) {
            if (isCollision(tg, player, i)) {
                gameOver = true;
                printf("You lose.\n");
                break;
            }
        }

        if (isCollision(tg, player, latticeSize * latticeSize - 1)) {
            gameOver = true;
            printf("You win.\nTime: %.3lf seconds", glfwGetTime() - startTime);
        }

        if (gameOver)
            quitLoop(tg, bg);

        glfwPollEvents();
        //glfwWaitEvents();

        // player moving
        if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            tg.setRot(player, glm::mod(tg.getRot(player) - angSpeed, 2.0f * glm::pi<GLfloat>()));
        } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            tg.setRot(player, glm::mod(tg.getRot(player) + angSpeed, 2.0f * glm::pi<GLfloat>()));
        } else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            tg.setPos(player, tg.getPos(player) + tg.getRotMat(player) * glm::vec2(0.0f, speed));
        } else if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            tg.setPos(player, tg.getPos(player) - tg.getRotMat(player) * glm::vec2(0.0f, speed));
        }
    } while(glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 &&
            !gameOver);
}

void MyWin::quitLoop(TriangleGrid &tg, Background &bg, double quitTime) {
    double start = glfwGetTime();
    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("quit-loopbegin");
        // =====================================================        Drawing
        bg.draw(aspect, glfwGetTime() - start, quitTime, true);
        tg.draw(aspect, glfwGetTime() - start, quitTime, true);
        AGLErrors("quit-afterdraw");

        WaitForFixedFPS();
        glfwSwapBuffers(win()); // =============================   Swap buffers

        glfwPollEvents();
        //glfwWaitEvents();
    } while (glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0 &&
             glfwGetTime() - start < quitTime);
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
