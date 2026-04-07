#ifndef DRAWABLE3D_H_INCLUDED
#define DRAWABLE3D_H_INCLUDED

#include "GLPlatform.h"
#include "Resources.h"
#include "GraphicsObject.h"
#include "Mesh3D.h"
#include "Shader.h"

namespace GGE {

    class Drawable3D : public GraphicsObject
    {
        public:
            Drawable3D(Shader* _shader);
            virtual ~Drawable3D();
            void loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices);
            void loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices,
                              std::vector<glm::vec2> _uvs);
            void loadGraphics(const char * objName, const resourceFile *model, const resourceFile *fileBuffer);
            void loadGraphics(const char * objName, std::vector<glm::vec3> _vertices);
            void loadGraphics(const char * objName, const resourceFile *model);
            void updateUV(std::vector<glm::vec2> uvs);
            GLuint loadTexture(const resourceFile *fileBuffer);
            virtual void draw();
            inline Shader* getShader() { return getMaterialShader(); };
            inline void setDepthMVP(glm::mat4 _depthMVP) { depthMVP = _depthMVP; };
            void setGLPosition(float offSet[16]);
            inline void setTexture(const GLuint _texture){ setMaterialTexture(_texture); };
            inline GLuint getTexture(){ return getMaterialTexture(); }
            void setMeshReference(Mesh3D* mesh);
            inline void setScale(const Vector3 _scaleMatrix)
            {
                scale = _scaleMatrix;
                ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(_scaleMatrix.x, _scaleMatrix.y, _scaleMatrix.z));
            }
            inline glm::mat4 getScale() { return ScaleMatrix; }
            inline GLuint getVertexBuffer() { return getMesh() ? getMesh()->getVertexBuffer() : 0; }
            inline GLuint getElementBuffer() { return getMesh() ? getMesh()->getElementBuffer() : 0; }
            inline GLuint getUVBuffer() { return getMesh() ? getMesh()->getUVBuffer() : 0; }
            inline GLsizei getIndexCount() { return getMesh() ? getMesh()->getIndexCount() : 0; }
            glm::mat4 buildModelMatrix() const;

        protected:
            float glMatrix[16];
            static const glm::mat4 biasMatrix;
            void    bindShadersVars();
            GLuint   vertexArrayID;
            GLuint   vertexPositionID;
            GLuint   vertexUVID;
            GLuint   vertexNormalID;
            bool ownsMesh;
            glm::mat4 ProjectionMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 ModelMatrix;
            glm::mat4 MVP;
            glm::mat4 ScaleMatrix;
            glm::mat4 depthMVP;
            std::vector<glm::vec2> uvs;
            std::vector<glm::vec3> dynVertices;
            std::vector<glm::vec3> normalVertices;
            bool updateSoft;
    };

}

#endif // DRAWABLE3D_H_INCLUDED