#include "../include/GraphicsObjectGL.h"

namespace GGE
{

//    GraphicsObject::GraphicsObject(unsigned short _shaderType)
//    {
//        shaderType = _shaderType;
//    }

    const glm::mat4 GraphicsObjectGL::biasMatrix = glm::mat4 (
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    GraphicsObjectGL::GraphicsObjectGL(Shader* _shader) : GraphicsObject()
    {
        visible=false;
        vertexPositionID = 0;
        vertexUVID = 1;
        vertexNormalID = 2;
        ScaleMatrix = glm::mat4(1.0f);
        alpha=1.;
        shader = _shader;

        std::fill(glMatrix, glMatrix+sizeof(glMatrix)/sizeof(float), 0);

		vertexBuffer = 0;
		uvBuffer = 0;
		normalBuffer = 0;
		texture = 0;

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);
    }

    GraphicsObjectGL::~GraphicsObjectGL()
    {
		if (vertexBuffer)
			glDeleteBuffers(1, &vertexBuffer);
		if (uvBuffer)
			glDeleteBuffers(1, &uvBuffer);
		if (normalBuffer)
			glDeleteBuffers(1, &normalBuffer);
		if (texture)
			glDeleteTextures(1, &texture);
		if (elementbuffer)
			glDeleteTextures(1, &elementbuffer);
    }

    void GraphicsObjectGL::setGLPosition(float offSet[16])
    {
        memcpy(glMatrix, offSet, 16 * sizeof(float));
    }

    // E.G. Normal Shader
    void GraphicsObjectGL::loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices)
    {
        //vertexPositionID = glGetAttribLocation(shader->getShaderID(), "vertexPos");

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vector3), &_vertices[0], GL_STATIC_DRAW);

        qtdIndices = _indices.size();

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned short), &_indices[0], GL_STATIC_DRAW);

    }

    // E.G. Normal Shader
    void GraphicsObjectGL::loadGraphics(const char * objName, std::vector<glm::vec3> _vertices)
    {
        vertexPositionID = glGetAttribLocation(shader->getShaderID(), "vertexPosition_modelspace");


        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

        Graphics::getInstance()->addGraphicsObject(objName, static_cast<GraphicsObjectGL*>(this));
    }

    // E.G. 3D Shader
    void GraphicsObjectGL::loadGraphics(const char * objName, const resourceFile *model, const resourceFile *fileBuffer)
    {
//        if (to_string(fileBuffer->fileName).find(".DDS") == std::string::npos)
//        {
            texture = GraphicsUtils::loadImage(fileBuffer);
//        }
//        else if (to_string(fileBuffer->fileName).find(".bmp") != std::string::npos)
//        {
//            texture = GraphicsUtils::loadBMP(fileBuffer);
//        }
//        else
//        {
//            texture = GraphicsUtils::loadDDS(fileBuffer);
//        }
        loadGraphics(objName, model);
    }

    GLuint GraphicsObjectGL::loadTexture(const resourceFile *fileBuffer)
    {
//        return GraphicsUtils::loadDDS(fileBuffer);
        return GraphicsUtils::loadImage(fileBuffer);
    }

    // E.G. 3D Shader whitout texture
    void GraphicsObjectGL::loadGraphics(const char * objName, const resourceFile *model)
    {

        vertexPositionID = glGetAttribLocation(shader->getShaderID(), "vertexPosition_modelspace");
        vertexUVID = glGetAttribLocation(shader->getShaderID(), "vertexUV");
        vertexNormalID = glGetAttribLocation(shader->getShaderID(), "vertexNormal_modelspace");

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        GraphicsUtils::loadOBJ(model, vertices, uvs, normals);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

        Graphics::getInstance()->addGraphicsObject(objName, static_cast<GraphicsObjectGL*>(this));
    }

//    void GraphicsObject::loadGraphics(const char * objName, const char * model)
//    {
////
////        glGenVertexArrays(1, &vertexArrayID);
////        glBindVertexArray(vertexArrayID);
//
//        std::vector<glm::vec3> vertices;
//        std::vector<glm::vec2> uvs;
//        std::vector<glm::vec3> normals;
//        GraphicsUtils::loadOBJ(model, vertices, uvs, normals);
//
//        qtdVertices = vertices.size();
//        glGenBuffers(1, &vertexBuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//        glBufferData(GL_ARRAY_BUFFER, qtdVertices * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
//
//        glGenBuffers(1, &uvBuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
//        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
//
//        glGenBuffers(1, &normalBuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
//        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
//
//        Graphics::getInstance()->addGraphicsObject(objName, static_cast<GraphicsObject*>(this));
//    }

    void GraphicsObjectGL::bindShadersVars()
    {

        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), "V"), 1, GL_FALSE, &ViewMatrix[0][0]);

        if (texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(shader->getShaderID(), "shaderTexture"), 0);


//            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP,GL_TRUE);
//                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }

//
//    void GraphicsObject::setSoftBody(const btSoftBody *_softBody)
//    {
//       softBody =  &_softBody;
//    }

    void GraphicsObjectGL::draw()
    {

		ProjectionMatrix = Graphics::getInstance()->getProjectionMatrix();
		ViewMatrix = Graphics::getInstance()->getViewMatrix();
		ModelMatrix = glm::translate(glm::mat4(), glm::vec3((float) position.x,// * viewportSize.x / SCREEN_X,
                                                            (float) position.y,// * viewportSize.y / SCREEN_Y,
                                                            (float) position.z))// * viewportSize.y / SCREEN_Y))
                * glm::rotate(orientation.w, glm::vec3(orientation.x, orientation.y, orientation.z))
                * glm::scale(glm::vec3(scale.x, scale.y, scale.z));

//		ModelMatrix = glm::mat4(1.0f);//x4(glMatrix[0], glMatrix[1], glMatrix[2], glMatrix[3],
////                                         glMatrix[4], glMatrix[5], glMatrix[6], glMatrix[7],
////                                         glMatrix[8], glMatrix[9], glMatrix[10], glMatrix[11],
////                                         glMatrix[12], glMatrix[13], glMatrix[14], glMatrix[15]) * ScaleMatrix;


		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        bindShadersVars();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,     // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

//        glEnableVertexAttribArray(vertexUVID);
//        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
//        glVertexAttribPointer(
//            vertexUVID,                       // attribute
//            2,                                // size
//            GL_FLOAT,                         // type
//            GL_FALSE,                         // normalized?
//            0,                                // stride
//            (void*)0                          // array buffer offset
//        );

            // if (shaderType == PV_UVSHADER)
            // {


            //     // 3rd attribute buffer : normals
            //     glEnableVertexAttribArray(vertexNormalID);
            //     glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
            //     glVertexAttribPointer(
            //         vertexNormalID,                   // attribute
            //         3,                                // size
            //         GL_FLOAT,                         // type
            //         GL_FALSE,                         // normalized?
            //         0,                                // stride
            //         (void*)0                          // array buffer offset
            //     );

            // }


            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

            glDrawElements(
                GL_TRIANGLES,      // mode
                qtdIndices,    // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
            );

        glDisableVertexAttribArray(0);
//        glDisableVertexAttribArray(vertexUVID);

        // else {
        //     glDrawArrays(GL_QUADS, 0, qtdVertices );
        //     glDisableVertexAttribArray(vertexPositionID);

        // }
		//glActiveTexture(0);

    }

}
