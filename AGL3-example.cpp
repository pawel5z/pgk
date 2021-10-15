// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>

#include "AGL3Window.hpp"
#include "AGL3Drawable.hpp"
#include "CirclePolygon.hpp"

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class MyTri : public AGLDrawable {
public:
   MyTri() : AGLDrawable(0) {
      setShaders();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         out vec2 pos;

         void main(void) {
            const vec2 vertices[3] = vec2[3](vec2( 0.9, -0.9),
                                             vec2(-0.9, -0.9),
                                             vec2( 0.9,  0.9));
            const vec4 colors[]    = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),
                                             vec4(0.0, 1.0, 0.0, 1.0),
                                             vec4(0.0, 0.0, 1.0, 1.0));

            vcolor      = colors[gl_VertexID];
            pos = vertices[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0);
         }

      )END", R"END(

         #version 330 
         in  vec4 vcolor;
         in vec2 pos;

         out vec4 color;

         void main(void) {
            if (distance(pos, vec2(0.0, 0.0)) <= 0.25)
                color = vec4(1.0, 1.0, 1.0, 1.0);
            else
                color = vcolor;
         } 

      )END");
   }
   void draw() {
      bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
};


// ==========================================================================
// Drawable object with some data in buffer and vertex/fragment programs
// ==========================================================================
class MyCross : public AGLDrawable {
public:
    GLfloat armLength;

   MyCross(GLfloat armLength) : AGLDrawable(0) {
       if (armLength <= 0)
           throw std::invalid_argument("armLength must be non-negative");
       this->armLength = armLength;
      setShaders();
      setBuffers();
   }
   void setShaders() {
      compileShaders(R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) in vec2 pos;
         layout(location = 0) uniform float scale;
         layout(location = 1) uniform vec2  center;
         out vec4 vtex;

         void main(void) {
            vec2 p = (pos * scale + center);
            gl_Position = vec4(p, 0.0, 1.0);
         }

      )END", R"END(

         #version 330
         #extension GL_ARB_explicit_uniform_location : require
         layout(location = 3) uniform vec3  cross_color;
         out vec4 color;

         void main(void) {
            color = vec4(cross_color,1.0);
         }

      )END");
   }
   void setBuffers() {
      bindBuffers();
      GLfloat vert[4][2] = {  // Cross lines
         { -1,  0  },
         {  1,  0  },
         {  0, -1  },
         {  0,  1  }
      };

      glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), vert, GL_STATIC_DRAW );
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(
         0,                 // attribute 0, must match the layout in the shader.
         2,                  // size
         GL_FLOAT,           // type
         GL_FALSE,           // normalized?
         0,//24,             // stride
         (void*)0            // array buffer offset
      );
   }
   void draw() {
      bindProgram();
      bindBuffers();
      glUniform1f(0, armLength);  // scale  in vertex shader
      glUniform2f(1, x, y);  // center in vertex shader
      glUniform3f(3, cross_color[0],cross_color[1],cross_color[2]);

      glDrawArrays(GL_LINES, 0, 4);
   }
   void setColor(float r, float g, float b){
      cross_color[0]=r;cross_color[1]=g;cross_color[2]=b;
   }
 private:
   GLfloat cross_color[3] = { 0.0, 1.0, 0.0 };
};


// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr=0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void KeyCB(int key, int scancode, int action, int mods);
    void MainLoop();
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
bool isCollision(const MyCross& cross, const CirclePolygon& circle) {
    GLfloat hx1 = cross.x - cross.armLength, hx2 = cross.x + cross.armLength;
    GLfloat vy1 = cross.y + cross.armLength, vy2 = cross.y - cross.armLength;

    //// check vertical cross bar collision
    GLfloat vDist = std::abs(cross.y - circle.y);
    // circle center between horizontal ends of the cross
    if (vDist <= circle.radius && hx1 <= circle.x && circle.x <= hx2)
        return true;
    // circle center on the left side of the cross
    if (glm::distance(glm::vec2(hx1, cross.y), glm::vec2(circle.x, circle.y)) <= circle.radius)
        return true;
    // circle center on the right side of the cross
    if (glm::distance(glm::vec2(hx2, cross.y), glm::vec2(circle.x, circle.y)) <= circle.radius)
        return true;

    //// check horizontal cross bar collision
    GLfloat hDist = std::abs(cross.x - circle.x);
    // circle center between vertical ends of the cross
    if (hDist <= circle.radius && vy1 <= circle.y && circle.y <= vy2)
        return true;
    // circle center above the cross
    if (glm::distance(glm::vec2(cross.x, vy1), glm::vec2(circle.x, circle.y)) <= circle.radius)
        return true;
    // circle center under the cross
    if (glm::distance(glm::vec2(cross.x, vy2), glm::vec2(circle.x, circle.y)) <= circle.radius)
        return true;

    return false;
}

// ==========================================================================
void MyWin::MainLoop() {
   ViewportOne(0,0,wd,ht);

   MyCross cross(1.0/16);
   cross.x = 0.0; cross.y = 0.5;
   MyTri   trian;
   CirclePolygon circlePoly(64, 1.0/16);
   circlePoly.x = -0.5; circlePoly.y = 0.5;

   do {
      glClear( GL_COLOR_BUFFER_BIT );
   
      AGLErrors("main-loopbegin");
      // =====================================================        Drawing
      trian.draw();
      cross.draw();
      circlePoly.draw();
      AGLErrors("main-afterdraw");

      glfwSwapBuffers(win()); // =============================   Swap buffers
      glfwPollEvents();
      //glfwWaitEvents();   

      // triangle filling
      if (glfwGetKey(win(), GLFW_KEY_DOWN ) == GLFW_PRESS) {
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else if (glfwGetKey(win(), GLFW_KEY_UP ) == GLFW_PRESS) {
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     // cross moving
      } else if (glfwGetKey(win(), GLFW_KEY_RIGHT ) == GLFW_PRESS) {
         cross.x += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_LEFT ) == GLFW_PRESS) {
         cross.x -= 0.01;
     // circle moving
      } else if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS) {
         circlePoly.y += 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS) {
         circlePoly.y -= 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS) {
         circlePoly.x -= 0.01;
      } else if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS) {
         circlePoly.x += 0.01;
      }
   } while( glfwGetKey(win(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
            glfwWindowShouldClose(win()) == 0 &&
            !isCollision(cross, circlePoly));
}

int main(int argc, char *argv[]) {
   MyWin win;
   win.Init(800,600,"AGL3 example",0,33);
   win.MainLoop();
   return 0;
}
