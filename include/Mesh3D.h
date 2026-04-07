#ifndef MESH3D_H_INCLUDED
#define MESH3D_H_INCLUDED

#include "Definitions.h"
#include "GLPlatform.h"

#include <vector>

namespace GGE
{

class Mesh3D
{
public:
    Mesh3D();
    ~Mesh3D();

    void setIndexedGeometry(const std::vector<Vector3>& vertices,
                            const std::vector<unsigned short>& indices);
    void setIndexedGeometry(const std::vector<Vector3>& vertices,
                            const std::vector<unsigned short>& indices,
                            const std::vector<glm::vec2>& uvs);
    void setPositionOnlyGeometry(const std::vector<glm::vec3>& vertices);
    void setObjGeometry(const std::vector<glm::vec3>& vertices,
                        const std::vector<glm::vec2>& uvs,
                        const std::vector<glm::vec3>& normals);
    // Atualiza apenas o UV buffer (sem recriar vértices/índices).
    // Usa GL_DYNAMIC_DRAW pois é chamado todo frame para animação.
    void updateUVBuffer(const std::vector<glm::vec2>& uvs);

    GLuint getVertexBuffer() const { return vertexBuffer; }
    GLuint getElementBuffer() const { return elementBuffer; }
    GLuint getUVBuffer() const { return uvBuffer; }
    GLuint getNormalBuffer() const { return normalBuffer; }
    GLsizei getIndexCount() const { return indexCount; }

private:
    void replaceBuffer(GLuint& buffer, GLenum target, GLsizeiptr size, const void* data);

    GLuint vertexBuffer = 0;
    GLuint elementBuffer = 0;
    GLuint uvBuffer = 0;
    GLuint normalBuffer = 0;
    GLsizei indexCount = 0;
};

}

#endif // MESH3D_H_INCLUDED