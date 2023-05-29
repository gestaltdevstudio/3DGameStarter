#include "../include/GraphicsUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace GGE
{
    void GraphicsUtils::calculateViewportSize(Point &viewport)
    {
        if (viewport.x / FIXED_ASPECT_RATIO < viewport.y)
        {
            viewport.y = (int) (viewport.x / FIXED_ASPECT_RATIO);
        }
        else
        {
            viewport.x = (int) (viewport.y * FIXED_ASPECT_RATIO);
        }
    }

    GLuint GraphicsUtils::loadImage(const resourceFile *fileBuffer)
    {
		GLuint imageID;

        int width, height, format;

        stbi_set_flip_vertically_on_load(true);
        unsigned char * data = stbi_load_from_memory((unsigned char*)fileBuffer->file, fileBuffer->size, &width, &height, &format, STBI_rgb_alpha);

		glGenTextures(1,&imageID);
		glBindTexture(GL_TEXTURE_2D,imageID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
		data=NULL;

		return imageID;
	}

    GLuint GraphicsUtils::loadShaders(const resourceFile *vertexFileBuffer, const resourceFile *fragmentFileBuffer)
    {

        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compile Vertex Shader
        GLchar* tmpV = static_cast< GLchar*>(vertexFileBuffer->file);
        tmpV[vertexFileBuffer->size-1] = '\0';
        GLchar const* VertexSourcePointer = tmpV;

        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
//            LOGI("Error GL: %s" , &VertexShaderErrorMessage[0]);
        }



        // Compile Fragment Shader
        GLchar* tmpF = static_cast< GLchar*>(fragmentFileBuffer->file);
        tmpF[fragmentFileBuffer->size-1] = '\0';
        GLchar const * FragmentSourcePointer = tmpF;

        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
//            LOGI("Error GL: %s" , &FragmentShaderErrorMessage[0]);
        }

        // Link the program
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if ( InfoLogLength > 0 ){
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
//            LOGI("Error GL: %s" , &ProgramErrorMessage[0]);
        }

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

    void GraphicsUtils::loadOBJ(const resourceFile *fileBuffer,
                  std::vector<glm::vec3> & out_vertices,
                  std::vector<glm::vec2> & out_uvs,
                  std::vector<glm::vec3> & out_normals)
    {
        //printf("Loading OBJ file %s...\n", fileBuffer.fileName);

        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;

        char * file;
        file = strtok ( static_cast<char*>(fileBuffer->file),"\r\n");

        char lineHeader[128];

        while( file != NULL)
        {

            // read the first word of the line
            sscanf(file, "%s", lineHeader);


            if ( strcmp( lineHeader, "v" ) == 0 ){
                glm::vec3 vertex;
                sscanf(file, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                glm::vec2 uv;
                sscanf(file, "vt %f %f", &uv.x, &uv.y );
                uv.y=1.0f-uv.y;
                temp_uvs.push_back(uv);
            }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                glm::vec3 normal;
                sscanf(file, "vn %f %f %f", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            }
            else if ( strcmp( lineHeader, "f" ) == 0 ){
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = sscanf(file, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    //std::cout << matches << std::endl;
                    printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices    .push_back(uvIndex[0]);
                uvIndices    .push_back(uvIndex[1]);
                uvIndices    .push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            else{
                // Probably a comment, eat up the rest of the line
                char stupidBuffer[1000];
                Resources::getInstance()->sgets(stupidBuffer, 1000, &file);
            }


            file = strtok ( NULL,"\r\n");
        }

        // For each vertex of each triangle
        for( unsigned int i=0; i<vertexIndices.size(); i++ ){

            // Get the indices of its attributes
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];

            // Get the attributes thanks to the index
            glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
            glm::vec2 uv = temp_uvs[ uvIndex-1 ];
            glm::vec3 normal = temp_normals[ normalIndex-1 ];

            // Put the attributes in buffers
            out_vertices.push_back(vertex);
            out_uvs     .push_back(uv);
            out_normals .push_back(normal);

        }
    }
}
