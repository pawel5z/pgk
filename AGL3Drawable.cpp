#include "AGL3Drawable.hpp"

AGLDrawable::AGLDrawable(GLuint _pid) {
    pId=_pid;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(     1, &vboId);
    glGenBuffers(     1, &eboId);
}

AGLDrawable::~AGLDrawable() {    // Cleanup VBO,VBO,Prog
#ifdef DEBUG
    fprintf(stderr,"AGL: destroying Drawable\n");
#endif
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &eboId);
    glDeleteVertexArrays(1, &vaoId);
    if (pId) glDeleteProgram(pId);
}

int AGLDrawable::compileShaders(const char *vs, const char *fs, const char *gs) {
    GLuint  v = glCreateShader(GL_VERTEX_SHADER);
    GLuint  f = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint  g = 0;
    if (gs) g = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(v, 1, &vs, nullptr);   // Also read from file: next fun
    glShaderSource(f, 1, &fs, nullptr);   // ...
    if (gs) glShaderSource(g, 1, &gs, nullptr);   // ...

    int res = compileShaders(v,f,g);
    glUseProgram(p());
    return res;
}

int AGLDrawable::compileShadersFromFile(const char *vs, const char *fs, const char *gs) {
    GLuint  v = glCreateShader(GL_VERTEX_SHADER);
    GLuint  f = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint  g = 0;
    if (gs) g = glCreateShader(GL_GEOMETRY_SHADER);
    getShaderSource(v, vs);
    getShaderSource(f, fs);
    if (gs) getShaderSource(g, gs);

    int res = compileShaders(v,f,g);
    glUseProgram(p());
    return res;
}

void AGLDrawable::bindVAO() const {
    glBindVertexArray(vaoId);
}

void AGLDrawable::bindBuffers() const {
    glBindVertexArray(            vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
}

void AGLDrawable::bindProgram() const {
    glUseProgram(pId);
}

GLuint AGLDrawable::p() const {
    return pId;
}

int AGLDrawable::compileShaders(GLuint v, GLuint f, GLuint g) {
    GLint Result = GL_FALSE;
    if (g) Result = CompileLink(g, "GS");
    if ( (Result = CompileLink(v, "VS")) )
        if ( CompileLink(f, "FS") ) {
            pId = glCreateProgram();
            glAttachShader(pId,v);
            glAttachShader(pId,f);
            if (g) glAttachShader(pId,g);
            CompileLink(pId, "Linking",  3);
        }
    glDeleteShader(v);
    glDeleteShader(f);
    if (g) glDeleteShader(g);
    return Result;
}

GLint AGLDrawable::CompileLink(GLuint v, const char *which, int prog) {
    GLint Result = GL_FALSE;
    int InfoLogLength;
    if (prog) {
        glLinkProgram(v);
        glGetProgramiv(v, GL_LINK_STATUS, &Result);
        glGetProgramiv(v, GL_INFO_LOG_LENGTH, &InfoLogLength);
    } else {
        glCompileShader(v);
        glGetShaderiv(v, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(v, GL_INFO_LOG_LENGTH, &InfoLogLength);
    }
    if ( InfoLogLength > 0 && !Result ) {
        std::vector<char> Message(InfoLogLength+1);
        if (prog)
            glGetProgramInfoLog(v,InfoLogLength,nullptr,&Message[0]);
        else
            glGetShaderInfoLog(v,InfoLogLength,nullptr,&Message[0]);
        printf("%s: %s\n", which, &Message[0]);
    }
    return Result;
}

void AGLDrawable::getShaderSource(GLuint sId, const char * file) {
    std::string sCode;
    std::ifstream sStream(file, std::ios::in);
    if(sStream.is_open()) {
        std::string Line = "";
        while(getline(sStream, Line))
            sCode += "\n" + Line;
        sStream.close();
    } else {
        printf("Error opening file:  %s !\n", file);
        getchar();
        return ;
    }
    char const * SourcePointer = sCode.c_str();
    glShaderSource(sId, 1, &SourcePointer, nullptr);
}
