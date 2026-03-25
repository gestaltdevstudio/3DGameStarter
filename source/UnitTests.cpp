#include "../include/UnitTests.h"
#include "../include/TextureRenderTarget.h"
#include "../include/UIDrawable.h"
#include "../include/UIWidget.h"
#include <iostream>

namespace GGE
{

bool runUnitTests()
{
    bool ok = true;

    // 1) Render target instantiation
    TextureRenderTarget* rt = new TextureRenderTarget(64, 64);
    if (rt->getFramebuffer() == 0 || rt->getTexture() == 0)
    {
        std::cerr << "[UnitTests] TextureRenderTarget init failed" << std::endl;
        ok = false;
    }
    rt->bind();
    rt->unbind();
    delete rt;

    // 2) UIDrawable properties
    UIDrawable* uiDrawable = new UIDrawable();
    uiDrawable->setIsVisible(true);
    if (!uiDrawable->isVisible())
    {
        std::cerr << "[UnitTests] UIDrawable visibility failed" << std::endl;
        ok = false;
    }
    delete uiDrawable;

    // 3) UIWidget hit-test and render path
    UIWidget widget;
    widget.setHitArea(10, 10, 100, 100);
    if (!widget.hitTest(50, 50) || widget.hitTest(0,0))
    {
        std::cerr << "[UnitTests] UIWidget hitTest failed" << std::endl;
        ok = false;
    }

    std::cout << "[UnitTests] " << (ok ? "OK" : "FAILED") << std::endl;
    return ok;
}

}
