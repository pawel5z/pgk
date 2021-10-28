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
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "TriangleObject.hpp"

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

/* Interpolate (i, j) from {0..n}x{0..n}
 * to [-0.9, 0.9]x[-0.9, 0.9]
 */
inline glm::vec2 onBoard(int i, int j, int n) {
    return {-0.9f + (GLfloat)i / (GLfloat)(n-1) * 1.8f,
            -0.9f + (GLfloat)j / (GLfloat)(n-1) * 1.8f};
}

inline GLfloat randAngle() {
    return (GLfloat)(rand() % 360) * glm::pi<GLfloat>() / 180.0f;
}

// ==========================================================================
void MyWin::MainLoop() {
    ViewportOne(0,0,wd,ht);

    int n = 10;
    std::unique_ptr<std::vector<std::unique_ptr<TriangleObject>>> ts(
            new std::vector<std::unique_ptr<TriangleObject>>(0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ts->push_back(std::make_unique<TriangleObject>(onBoard(i, j, n), randAngle(), 1.0f/12.0f));
        }
    }
    std::unique_ptr<TriangleObject> &player = ts->at(0);
    player->setVertexColor(0, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    player->setVertexColor(1, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    player->setVertexColor(2, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    GLfloat speed = 0.005;
    GLfloat angSpeed = 0.02;

    do {
        glClear( GL_COLOR_BUFFER_BIT );

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        for (auto const &t : *ts) {
            t->draw();
        }
        AGLErrors("main-afterdraw");

        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        //glfwWaitEvents();

        // player moving
        if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
            player->setRot(glm::mod(player->getRot() - angSpeed, 2.0f * glm::pi<GLfloat>()));
        } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
            player->setRot(glm::mod(player->getRot() + angSpeed, 2.0f * glm::pi<GLfloat>()));
        } else if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            player->pos += player->getRotMat() * glm::vec2(0.0f, speed);
        } else if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            player->pos += player->getRotMat() * glm::vec2(0.0f, -speed);
        }
    } while(glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    MyWin win;
    win.Init(800,600,"AGL3 example",0,33);
    win.MainLoop();
    return 0;
}
