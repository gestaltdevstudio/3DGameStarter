#ifndef UIDRAWABLE_H_INCLUDED
#define UIDRAWABLE_H_INCLUDED

#include "Drawable.h"

namespace GGE
{

class UIDrawable : public Drawable
{
public:
    UIDrawable() = default;
    virtual ~UIDrawable() override = default;

    void setVisible(bool v) { setIsVisible(v); }
    bool isVisible() const { return Drawable::isVisible(); }
};

}

#endif // UIDRAWABLE_H_INCLUDED
