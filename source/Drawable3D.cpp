#include "../include/Drawable3D.h"
#include "../include/GraphicsManager.h"
#include "../include/Graphics.h"

namespace GGE
{

//    GraphicsObject::GraphicsObject(unsigned short _shaderType)
//    {
//        shaderType = _shaderType;
//    }

    const glm::mat4 Drawable3D::biasMatrix = glm::mat4 (
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    Drawable3D::Drawable3D(Shader* _shader) : GraphicsObject()
    {
        visible=false;
        vertexPositionID = 0;
        vertexUVID = 1;
        vertexNormalID = 2;
        ScaleMatrix = glm::mat4(1.0f);
        alpha=1.;
        setMaterialShader(_shader);
        ownsMesh = true;
        setMesh(new Mesh3D());

        std::fill(glMatrix, glMatrix+sizeof(glMatrix)/sizeof(float), 0);
		setMaterialTexture(0);

        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);
    }

    Drawable3D::~Drawable3D()
    {
        GLuint textureId = getMaterialTexture();
        if (textureId)
            glDeleteTextures(1, &textureId);
		if (ownsMesh && getMesh())
			delete getMesh();
    }

    void Drawable3D::setMeshReference(Mesh3D* mesh)
    {
        if (ownsMesh && getMesh())
            delete getMesh();

        ownsMesh = false;
        setMesh(mesh);
    }

    void Drawable3D::setGLPosition(float offSet[16])
    {
        memcpy(glMatrix, offSet, 16 * sizeof(float));
    }

    // E.G. Normal Shader
    void Drawable3D::updateUV(std::vector<glm::vec2> uvs)
    {
        if (!getMesh())
            return;
        getMesh()->updateUVBuffer(uvs);
    }

    void Drawable3D::loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices)
    {
        if (!getMesh())
            return;

        getMesh()->setIndexedGeometry(_vertices, _indices);
    }

    void Drawable3D::loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices,
                                  std::vector<glm::vec2> _uvs)
    {
        if (!getMesh())
            return;

        getMesh()->setIndexedGeometry(_vertices, _indices, _uvs);
    }

    // E.G. Normal Shader
    void Drawable3D::loadGraphics(const char * objName, std::vector<glm::vec3> _vertices)
    {
        vertexPositionID = glGetAttribLocation(getShader()->getShaderID(), "vertexPosition_modelspace");


        if (getMesh())
            getMesh()->setPositionOnlyGeometry(_vertices);

        GraphicsManager::getInstance()->addGraphicsObject3D(objName, this);
    }

    // E.G. 3D Shader
    void Drawable3D::loadGraphics(const char * objName, const resourceFile *model, const resourceFile *fileBuffer)
    {
//        if (to_string(fileBuffer->fileName).find(".DDS") == std::string::npos)
//        {
            setTexture(GraphicsUtils::loadImage(fileBuffer));
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

    GLuint Drawable3D::loadTexture(const resourceFile *fileBuffer)
    {
//        return GraphicsUtils::loadDDS(fileBuffer);
        return GraphicsUtils::loadImage(fileBuffer);
    }

    // E.G. 3D Shader whitout texture
    void Drawable3D::loadGraphics(const char * objName, const resourceFile *model)
    {

        vertexPositionID = glGetAttribLocation(getShader()->getShaderID(), "vertexPosition_modelspace");
        vertexUVID = glGetAttribLocation(getShader()->getShaderID(), "vertexUV");
        vertexNormalID = glGetAttribLocation(getShader()->getShaderID(), "vertexNormal_modelspace");

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        GraphicsUtils::loadOBJ(model, vertices, uvs, normals);

        if (getMesh())
            getMesh()->setObjGeometry(vertices, uvs, normals);

        GraphicsManager::getInstance()->addGraphicsObject3D(objName, this);
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

    void Drawable3D::bindShadersVars()
    {

        glUniformMatrix4fv(glGetUniformLocation(getShader()->getShaderID(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), "V"), 1, GL_FALSE, &ViewMatrix[0][0]);

        if (getTexture())
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, getTexture());
            glUniform1i(glGetUniformLocation(getShader()->getShaderID(), "shaderTexture"), 0);


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

    void Drawable3D::draw()
    {

		ProjectionMatrix = Graphics::getInstance()->getProjectionMatrix();
		ViewMatrix = Graphics::getInstance()->getViewMatrix();
		ModelMatrix = buildModelMatrix();

//		ModelMatrix = glm::mat4(1.0f);//x4(glMatrix[0], glMatrix[1], glMatrix[2], glMatrix[3],
////                                         glMatrix[4], glMatrix[5], glMatrix[6], glMatrix[7],
////                                         glMatrix[8], glMatrix[9], glMatrix[10], glMatrix[11],
////                                         glMatrix[12], glMatrix[13], glMatrix[14], glMatrix[15]) * ScaleMatrix;


		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        bindShadersVars();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, getVertexBuffer());
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


            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getElementBuffer());

            glDrawElements(
                GL_TRIANGLES,      // mode
                getIndexCount(),    // count
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

        glm::mat4 Drawable3D::buildModelMatrix() const
        {
                const glm::mat4 translation = glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3((float)position.x, (float)position.y, (float)position.z));

                glm::mat4 rotation(1.0f);
                const glm::vec3 axis((float)orientation.x, (float)orientation.y, (float)orientation.z);
                if (glm::length2(axis) > 0.0f)
                {
                    rotation = glm::toMat4(glm::angleAxis(orientation.w, glm::normalize(axis)));
                }

                const glm::mat4 scaling = glm::scale(
                    glm::mat4(1.0f),
                    glm::vec3((float)scale.x, (float)scale.y, (float)scale.z));

                return translation * rotation * scaling;
        }

}
