#ifndef GRAPHICSOBJECTGL_H_INCLUDED
#define GRAPHICSOBJECTGL_H_INCLUDED

#include "Resources.h"
#include "Graphics.h"
#include "GraphicsObject.h"
#include "Shader.h"
#if defined(__ANDROID__)
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#elif defined(__WIN32__)
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#elif __APPLE__
 #include "TargetConditionals.h"
 #if TARGET_OS_OSX
  #include "OS_GLFW.h"
  #include <glad/glad.h>
 #else
  #include "OS_iOS.h"
 #endif
#else
 #include "OS_GLFW.h"
 #include <glad/glad.h>
#endif

namespace GGE {

    class GraphicsObjectGL : public GraphicsObject
	{
        public:
            GraphicsObjectGL(Shader* _shader);
            virtual ~GraphicsObjectGL();
            void loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices);
            void loadGraphics(const char * objName, const resourceFile *model, const resourceFile *fileBuffer);
            void loadGraphics(const char * objName, std::vector<glm::vec3> _vertices);
            void loadGraphics(const char * objName, const resourceFile *model);
            void updateUV(std::vector<glm::vec2> uvs);
            GLuint loadTexture(const resourceFile *fileBuffer);
            virtual void draw();
            inline Shader* getShader() { return shader; };
            inline void setDepthMVP(glm::mat4 _depthMVP) { depthMVP = _depthMVP; };
            void setGLPosition(float offSet[16]);
            inline void setTexture(const GLuint _texture){texture = _texture;};
            inline GLuint getTexture(){return texture;}
            inline void setScale(const Vector3 _scaleMatrix) {ScaleMatrix = (glm::mat4)glm::scale(_scaleMatrix.x, _scaleMatrix.y, _scaleMatrix.z); }
            inline glm::mat4 getScale() {return ScaleMatrix;}
            inline GLuint getVertexBuffer() { return vertexBuffer; }

        protected:

            int qtdIndices;
            Shader *shader;
            float glMatrix[16];
            static const glm::mat4 biasMatrix;
            void    bindShadersVars();
            GLuint   vertexArrayID;
            GLuint   vertexPositionID;
            GLuint   vertexUVID;
            GLuint   vertexNormalID;
            GLuint   uvBuffer;
            GLuint   normalBuffer;
            GLuint   texture;
            GLuint   vertexBuffer;
            GLuint   elementbuffer;
//            GLuint   matrixID;
//            GLuint   modelMatrixID;
//            GLuint   viewMatrixID;
//            GLuint   lightID;
//            GLuint   textureID;
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

#endif // GRAPHICSOBJECTGL_H_INCLUDED
