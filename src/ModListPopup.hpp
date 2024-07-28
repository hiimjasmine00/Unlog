#pragma once

#include <Geode/Geode.hpp>

class ModListPopup : public geode::Popup<> {

protected:
    bool setup() override;
    bool init(float width, float height);
public:
    static ModListPopup* create();
    void onClose(cocos2d::CCObject*);
};