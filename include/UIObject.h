#ifndef UIOBJECT_H_INCLUDED
#define UIOBJECT_H_INCLUDED

#include "Drawable2D.h"

namespace GGE
{

    class UIObject
    {

        public:
            UIObject();
            virtual ~UIObject();
            void setDrawable(Drawable2D *_drawable);
            inline Drawable2D* getDrawable() { return drawable; }
            inline void setPosition(int x, int y) { position->x = x; position->y = y;}
            inline void setDimension(int x, int y) { dimension->x = x; dimension->y = y;}

            inline Point* getPosition() { return position; }
            inline Point* getDimension() { return dimension; }


        protected:
            Drawable2D *drawable;
            Point *dimension;
            Point *position;

    };
}


#endif // UIOBJECT_H_INCLUDED
