#include "ImportedAsset.hpp"
#include "utils.hpp"

#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <exception>
#include <IL/devil_cpp_wrapper.hpp>
#include <iostream>

ImportedAsset::ImportedAsset(std::string path, std::string vertPath, std::string fragPath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);
    if (scene == nullptr)
        throw std::logic_error("Error while importing " + path + ".");

    glGenVertexArrays(1, &vArray);
    glBindVertexArray(vArray);
    std::vector<glm::vec3> vPositions;
    for (GLuint i = 0; i < scene->mNumMeshes; i++) {
        for (GLuint j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
            vPositions.push_back({
                scene->mMeshes[i]->mVertices[j].x,
                scene->mMeshes[i]->mVertices[j].y,
                scene->mMeshes[i]->mVertices[j].z
            });
            center += scene->mMeshes[i]->mVertices[j].x +
                      scene->mMeshes[i]->mVertices[j].y +
                      scene->mMeshes[i]->mVertices[j].z;
            greatestZ = scene->mMeshes[i]->mVertices[j].z > greatestZ ? scene->mMeshes[i]->mVertices[j].z : greatestZ;
        }
    }
    verticesCnt = vPositions.size();
    center /= (float)verticesCnt;
    std::cout << verticesCnt << " vertices.\n";
    glGenBuffers(1, &vPosBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vPosBuf);
    glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(glm::vec3), vPositions.data(), GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vArray, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), nullptr);

    compileProgramFromFile(program, vertPath.c_str(), fragPath.c_str());
}

struct VertexData {
    float x;
    float y;
    float z;
    float u;
    float v;
};

ImportedAsset::ImportedAsset(std::string path, std::string vertPath, std::string fragPath, std::string texPath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (scene == nullptr)
        throw std::logic_error("Error while importing " + path + ".");

    glGenVertexArrays(1, &vArray);
    glBindVertexArray(vArray);
    std::vector<VertexData> vPositions;
    for (GLuint i = 0; i < scene->mNumMeshes; i++) {
        for (GLuint j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
            vPositions.push_back({
                scene->mMeshes[i]->mVertices[j].x,
                scene->mMeshes[i]->mVertices[j].y,
                scene->mMeshes[i]->mVertices[j].z,
                scene->mMeshes[i]->mTextureCoords[0][j].x,
                scene->mMeshes[i]->mTextureCoords[0][j].y,
            });
            center += scene->mMeshes[i]->mVertices[j].x +
                      scene->mMeshes[i]->mVertices[j].y +
                      scene->mMeshes[i]->mVertices[j].z;
            greatestZ = scene->mMeshes[i]->mVertices[j].z > greatestZ ? scene->mMeshes[i]->mVertices[j].z : greatestZ;
        }
    }
    verticesCnt = vPositions.size();
    center /= (float)verticesCnt;
    std::cout << verticesCnt << " vertices.\n";
    glGenBuffers(1, &vPosBuf);
    glBindBuffer(GL_ARRAY_BUFFER, vPosBuf);
    glBufferData(GL_ARRAY_BUFFER, vPositions.size() * sizeof(VertexData), vPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexData), nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexData), (void *)(sizeof(float) * 3));

    ilImage img(texPath.c_str());
    img.Convert(IL_RGB);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.Width(), img.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    compileProgramFromFile(program, vertPath.c_str(), fragPath.c_str());
}

ImportedAsset::~ImportedAsset()
{
    if (vArray)
        glDeleteVertexArrays(1, &vArray);
    if (vPosBuf)
        glDeleteBuffers(1, &vPosBuf);
    if (texture)
        glDeleteTextures(1, &texture);
    if (program)
        glDeleteProgram(program);
}

void ImportedAsset::draw(const Camera &camera)
{
    glBindVertexArray(vArray);
    glUseProgram(program);
    glUniformMatrix4fv(0, 1, false, &(camera.getPVMat() * getModelMat())[0][0]);
    if (texture)
        glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, verticesCnt);
}

glm::vec3 ImportedAsset::getCenter() const
{
    return getModelMat() * glm::vec4(center, 1.f);
}

float ImportedAsset::getGreatestZ() const
{
    return (getModelMat() * glm::vec4(0.f, 0.f, greatestZ, 1.f)).z;
}
