#ifndef GRAPHICSOBJECT_H_INCLUDED
#define GRAPHICSOBJECT_H_INCLUDED

#include "Definitions.h"
#include "Entity.h"
#include <vector>

namespace GGE {

    class GraphicsObject
	{
        public:
            inline GraphicsObject() {
                position.x = position.y = position.z = 0;
                orientation.x = orientation.y = orientation.z = orientation.w = 0;
                scale.x = scale.y = scale.z = 1;
            };
            inline virtual ~GraphicsObject() { };
            virtual void draw() = 0;
            virtual void loadGraphics(std::vector<Vector3> _vertices, std::vector<unsigned short> _indices) = 0;
            inline void setPosition(const Vector3 _position) { position = _position; }
            inline void setOrientation(Quaternion _orientation) { orientation = _orientation; }
            inline void setColor(Vector4 _color) { color = _color; };
            inline void setVisible(const bool _visible) { visible = _visible; };
            inline bool isVisible() { return visible; };
            inline void setScale(const Vector3 _scale) { scale = _scale; }
            inline Vector3 getScale() { return scale; }
            inline void setAlpha(const float _alpha) { alpha=_alpha; }
            inline float getAlpha() { return alpha; }
            inline Vector3 getPosition() { return position; }
            inline void setRenderOrder(const unsigned int _renderOrder) { renderOrder = _renderOrder; }
            inline unsigned int getRenderOrder() {return renderOrder;}
            inline Entity* getEntity() { return entity;}
            inline void setEntity(Entity* _entity) { entity = _entity;}

        protected:

            Vector3 position;
            Vector3 scale;
            Quaternion orientation;
            Vector4 color;
            bool    visible;
            float   alpha;
            unsigned int  renderOrder;
            Entity  *entity;
    };

}

#endif // GRAPHICSOBJECT_H_INCLUDED
