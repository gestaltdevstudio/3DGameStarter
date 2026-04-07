#ifndef UIDRAWABLE_H_INCLUDED
#define UIDRAWABLE_H_INCLUDED

#include "Drawable2D.h"

namespace GGE
{

class UIDrawable : public Drawable2D
{
public:
    UIDrawable() = default;
    virtual ~UIDrawable() override = default;

    void setVisible(bool v) { setIsVisible(v); }
    bool isVisible() const { return Drawable2D::isVisible(); }
};

}

#endif // UIDRAWABLE_H_INCLUDED
