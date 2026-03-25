#ifndef UIWIDGET_H_INCLUDED
#define UIWIDGET_H_INCLUDED

#include <functional>
#include <memory>
#include "Definitions.h"
#include "UIDrawable.h"
#include "RenderPipeline.h"

namespace GGE
{

class UIWidget
{
public:
    UIWidget() = default;
    virtual ~UIWidget() = default;

    virtual void onPointerDown(int x, int y) {}
    virtual void onPointerUp(int x, int y) {}
    virtual void onPointerMove(int x, int y) {}

    virtual void render(RenderPipeline* pipeline)
    {
        if (visual) pipeline->draw(visual.get());
    }

    void setVisual(UIDrawable* v) { visual.reset(v); }
    UIDrawable* getVisual() const { return visual.get(); }

    void setHitArea(int x, int y, int width, int height)
    {
        bounds.x = x;
        bounds.y = y;
        bounds.w = width;
        bounds.h = height;
    }

    bool hitTest(int x, int y) const
    {
        return x >= bounds.x && x <= bounds.x + bounds.w
            && y >= bounds.y && y <= bounds.y + bounds.h;
    }

    void setOnClick(std::function<void()> cb) { onClick = std::move(cb); }

protected:
    struct Rect { int x; int y; int w; int h; } bounds{0,0,0,0};
    std::unique_ptr<UIDrawable> visual;
    std::function<void()> onClick;
};

}

#endif // UIWIDGET_H_INCLUDED
