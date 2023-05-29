#ifndef GRAPHICSUTILS_H_INCLUDED
#define GRAPHICSUTILS_H_INCLUDED

#include <vector>
#include "Resources.h"

namespace GGE {

    class GraphicsUtils
	{
        public:

          static GLuint loadImage(const resourceFile *fileBuffer);

          static void calculateViewportSize(Point &windowSize);

          static GLuint loadShaders(const resourceFile *vertexFileBuffer, const resourceFile *fragmentFileBuffer);

          static void loadOBJ(const resourceFile *fileBuffer,
                std::vector<glm::vec3> & out_vertices,
                std::vector<glm::vec2> & out_uvs,
                std::vector<glm::vec3> & out_normals);
    };

}

#endif // GRAPHICSUTILS_H_INCLUDED
