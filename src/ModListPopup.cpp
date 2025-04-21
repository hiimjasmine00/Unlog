#include "ModListPopup.hpp"
#include "ModList.hpp"
#include "UnlogData.hpp"

bool ModListPopup::init(float width, float height){
    bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");
    const char* bg = geodeTheme ? "geode.loader/GE_square01.png" : "GJ_square01.png";

    if (!Popup<>::initAnchored(width, height, bg)) return false;

    this->setCloseButtonSpr(
        CircleButtonSprite::createWithSpriteFrameName(
            "geode.loader/close.png", .85f,
            (geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
        )
    );

    return true;
}

bool ModListPopup::setup() {

    this->setTitle("Mod Logs:");
    m_title->limitLabelWidth(m_mainLayer->getContentSize().width - 50, .7f, .1f);

    ModList* list = ModList::create(Loader::get()->getAllMods(), {340.f, 150.f});

    m_mainLayer->addChildAtPosition(
        list,
        Anchor::Center,
        { 0.0f, -10.0f }
    );

    return true;
}

void ModListPopup::onClose(cocos2d::CCObject*){
    Mod::get()->setSavedValue("unlog-data", *UnlogData::data);
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
}

ModListPopup* ModListPopup::create() {
    auto ret = new ModListPopup();
    if (ret->init(380.f, 200.f)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}