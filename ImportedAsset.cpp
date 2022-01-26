#include "ImportedAsset.hpp"
#include "utils.hpp"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <exception>

ImportedAsset::ImportedAsset(std::string path, std::string vertPath, std::string fragPath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);
    if (scene == nullptr)
        throw std::logic_error("Error while importing " + path + ".");

    glGenVertexArrays(1, &vArray);
    glBindVertexArray(vArray);
    std::vector<glm::vec3> vPositions;
    for (GLuint i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {
        vPositions.push_back({
            scene->mMeshes[0]->mVertices[i].x,
            scene->mMeshes[0]->mVertices[i].y,
            scene->mMeshes[0]->mVertices[i].z
        });
    }
    verticesCnt = vPositions.size();
    glGenBuffers(1, &vPosBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vPosBuf);
    glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(glm::vec3), vPositions.data(), GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vArray, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    compileProgramFromFile(program, vertPath.c_str(), fragPath.c_str());
}

ImportedAsset::~ImportedAsset()
{
    if (vArray)
        glDeleteVertexArrays(1, &vArray);
    if (vPosBuf)
        glDeleteBuffers(1, &vPosBuf);
    if (program)
        glDeleteProgram(program);
}

void ImportedAsset::draw(const Camera &camera)
{
    glBindVertexArray(vArray);
    glUseProgram(program);
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, verticesCnt);
}
