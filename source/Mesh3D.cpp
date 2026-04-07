#include "../include/Mesh3D.h"


namespace GGE
{

Mesh3D::Mesh3D() = default;

Mesh3D::~Mesh3D()
{
    if (vertexBuffer)
        glDeleteBuffers(1, &vertexBuffer);
    if (elementBuffer)
        glDeleteBuffers(1, &elementBuffer);
    if (uvBuffer)
        glDeleteBuffers(1, &uvBuffer);
    if (normalBuffer)
        glDeleteBuffers(1, &normalBuffer);
}

void Mesh3D::replaceBuffer(GLuint& buffer, GLenum target, GLsizeiptr size, const void* data)
{
    if (!buffer)
        glGenBuffers(1, &buffer);

    glBindBuffer(target, buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
}

void Mesh3D::setIndexedGeometry(const std::vector<Vector3>& vertices,
                               const std::vector<unsigned short>& indices)
{
    if (vertices.empty() || indices.empty())
        return;

    replaceBuffer(vertexBuffer, GL_ARRAY_BUFFER,
                  vertices.size() * sizeof(Vector3), &vertices[0]);
    replaceBuffer(elementBuffer, GL_ELEMENT_ARRAY_BUFFER,
                  indices.size() * sizeof(unsigned short), &indices[0]);

    indexCount = static_cast<GLsizei>(indices.size());
}

void Mesh3D::setIndexedGeometry(const std::vector<Vector3>& vertices,
                               const std::vector<unsigned short>& indices,
                               const std::vector<glm::vec2>& uvs)
{
    if (vertices.empty() || indices.empty())
        return;

    replaceBuffer(vertexBuffer, GL_ARRAY_BUFFER,
                  vertices.size() * sizeof(Vector3), &vertices[0]);
    replaceBuffer(elementBuffer, GL_ELEMENT_ARRAY_BUFFER,
                  indices.size() * sizeof(unsigned short), &indices[0]);

    if (!uvs.empty())
    {
        replaceBuffer(uvBuffer, GL_ARRAY_BUFFER,
                      uvs.size() * sizeof(glm::vec2), &uvs[0]);
    }

    indexCount = static_cast<GLsizei>(indices.size());
}

void Mesh3D::updateUVBuffer(const std::vector<glm::vec2>& uvs)
{
    if (uvs.empty())
        return;

    if (!uvBuffer)
        glGenBuffers(1, &uvBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(uvs.size() * sizeof(glm::vec2)),
                 uvs.data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh3D::setPositionOnlyGeometry(const std::vector<glm::vec3>& vertices)
{
    if (vertices.empty())
        return;

    replaceBuffer(vertexBuffer, GL_ARRAY_BUFFER,
                  vertices.size() * sizeof(glm::vec3), &vertices[0]);
    indexCount = 0;
}

void Mesh3D::setObjGeometry(const std::vector<glm::vec3>& vertices,
                           const std::vector<glm::vec2>& uvs,
                           const std::vector<glm::vec3>& normals)
{
    if (!vertices.empty())
    {
        replaceBuffer(vertexBuffer, GL_ARRAY_BUFFER,
                      vertices.size() * sizeof(glm::vec3), &vertices[0]);
    }

    if (!uvs.empty())
    {
        replaceBuffer(uvBuffer, GL_ARRAY_BUFFER,
                      uvs.size() * sizeof(glm::vec2), &uvs[0]);
    }

    if (!normals.empty())
    {
        replaceBuffer(normalBuffer, GL_ARRAY_BUFFER,
                      normals.size() * sizeof(glm::vec3), &normals[0]);
    }

    indexCount = 0;
}

}