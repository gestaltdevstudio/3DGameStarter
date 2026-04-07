#ifndef GRAPHICSOBJECT_H_INCLUDED
#define GRAPHICSOBJECT_H_INCLUDED

#include "Definitions.h"
#include "Entity.h"
#include "Material3D.h"

#include <vector>

namespace GGE {

    class Mesh3D;

    class GraphicsObject
	{
        public:
            inline GraphicsObject() {
                position.x = position.y = position.z = 0;
                orientation.x = orientation.y = orientation.z = orientation.w = 0;
                scale.x = scale.y = scale.z = 1;
                color.r = color.g = color.b = color.a = 1.0f;
                visible = false;
                alpha = 1.0f;
                renderOrder = 0;
                entity = nullptr;
                mesh = nullptr;
            };
            inline virtual ~GraphicsObject() { };
            virtual void draw() = 0;
            virtual void loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices) = 0;
            inline void setPosition(const Vector3 _position) { position = _position; }
            inline void setOrientation(Quaternion _orientation) { orientation = _orientation; }
            inline void setColor(Vector4 _color) {
                color = _color;
                if (_color.a < 1.0f)
                    material.getRenderState().blendMode = BLEND_3D_ALPHA;
            };
            inline Vector4 getColor() const { return color; }
            inline void setVisible(const bool _visible) { visible = _visible; };
            inline bool isVisible() { return visible; };
            inline void setScale(const Vector3 _scale) { scale = _scale; }
            inline Vector3 getScale() { return scale; }
            inline Quaternion getOrientation() { return orientation; }
            inline void setAlpha(const float _alpha) {
                alpha = _alpha;
                material.getRenderState().blendMode =
                    (_alpha < 1.0f) ? BLEND_3D_ALPHA : BLEND_3D_DISABLED;
            }
            inline float getAlpha() { return alpha; }
            inline Vector3 getPosition() { return position; }
            inline void setRenderOrder(const unsigned int _renderOrder) { renderOrder = _renderOrder; }
            inline unsigned int getRenderOrder() {return renderOrder;}
            inline Entity* getEntity() { return entity;}
            inline void setEntity(Entity* _entity) { entity = _entity;}
            inline void setMaterialShader(Shader* _shader) { material.setShader(_shader); }
            inline Shader* getMaterialShader() const { return material.getShader(); }
            inline void setMaterialTexture(GLuint _texture) { material.setTextureId(_texture); }
            inline GLuint getMaterialTexture() const { return material.getTextureId(); }
            inline void setBlendMode(BlendMode3D _mode) { material.getRenderState().blendMode = _mode; }
            inline BlendMode3D getBlendMode() const { return material.getRenderState().blendMode; }
            inline void setDepthTestEnabled(bool enabled) { material.getRenderState().depthTestEnabled = enabled; }
            inline bool isDepthTestEnabled() const { return material.getRenderState().depthTestEnabled; }
            inline void setDepthWriteEnabled(bool enabled) { material.getRenderState().depthWriteEnabled = enabled; }
            inline bool isDepthWriteEnabled() const { return material.getRenderState().depthWriteEnabled; }
            inline unsigned int getRenderStateFlags() const { return material.getRenderStateFlags(); }
            inline Material3D& getMaterial() { return material; }
            inline const Material3D& getMaterial() const { return material; }
            inline void setMesh(Mesh3D* value) { mesh = value; }
            inline Mesh3D* getMesh() const { return mesh; }
            inline glm::mat4 buildModelMatrix() const
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

        protected:

            Vector3 position;
            Vector3 scale;
            Quaternion orientation;
            Vector4 color;
            bool    visible;
            float   alpha;
            unsigned int  renderOrder;
            Entity  *entity;
            Material3D material;
                Mesh3D* mesh;
    };

}

#endif // GRAPHICSOBJECT_H_INCLUDED
