#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <string>
#include "Definitions.h"

namespace GGE
{

    class Entity
    {

        public:
            inline Entity(std::string _name) { name = _name; 
				position = (Vector3*) malloc(sizeof(Vector3));
				size = (Vector3*) malloc(sizeof(Vector3));
				orientation = (Quaternion*) malloc(sizeof(Quaternion));

				position->x = position->y = position->z = 0;
				size->x = size->y = size->z = 1;
				orientation->x = orientation->y = orientation->z = orientation->w = 0;
		};
            inline ~Entity()
            {
                if (position) free(position);
                if (size) free(size);
                if (orientation) free(orientation);
            }

            inline Vector3* getPosition() { return position; }
            inline Vector3* getSize() { return size; }
            inline Quaternion* getOrientation() { return orientation; }

            inline void setPosition(float _x, float _y, float _z) { position->x = _x; position->y = _y; position->z = _z; }
            inline void setSize(float _x, float _y, float _z) { size->x = _x; size->y = _y; size->z = _z; }
            inline void setOrientation(float _x, float _y, float _z, float _w) { orientation->x = _x; orientation->y = _y; orientation->z = _z; orientation->w = _w; }
            inline std::string getName() { return name; };

        protected:
            Vector3    *position;
            Vector3    *size;
            Quaternion *orientation;
            std::string name;
    };


}

#endif // ENTITY_H_INCLUDED
