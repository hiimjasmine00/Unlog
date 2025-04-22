#include "ModItem.hpp"
#include "UnlogData.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/ui/GeodeUI.hpp>

bool ModItem::init(Mod* mod, CCSize const& size) {
    if (!CCNode::init()) {
        return false;
    }

    m_mod = mod;

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    ccColor4B bgColor = ColorProvider::get()->color("geode.loader/mod-developer-item-bg");

    m_bg = CCScale9Sprite::create("square02b_001.png");
    m_bg->setColor(to3B(bgColor));
    m_bg->setOpacity(bgColor.a);
    m_bg->setScale(.3f);
    m_bg->setContentSize(CCSize {
        size.width,
        size.height
    } / m_bg->getScale());
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    auto logoBtn = CCMenuItemExt::createSpriteExtra(
        geode::createModLogo(mod)->getChildByID("sprite"), [mod](CCNode*) {
            geode::openInfoPopup(mod);
        }
    );

    logoBtn->m_baseScale = (0.38f);
    logoBtn->setScale(logoBtn->m_baseScale);
    logoBtn->m_animationEnabled = false;
    logoBtn->m_colorEnabled = true;
    logoBtn->setAnchorPoint({ 0.0f, 0.5f });

    this->addChildAtPosition(
        CCMenu::createWithItem(logoBtn),
        Anchor::Left,
        { 4, 0 }
    );

    auto nameLabel = CCLabelBMFont::create(
        mod->getName().c_str(),
        "bigFont.fnt"
    );

    nameLabel->setScale(0.4f);
    nameLabel->setAnchorPoint({ 0.0f, 0.5f });

    #ifdef GEODE_IS_ARM_MAC
    constexpr float padding_left = 60.000f;
    #else
    constexpr float padding_left = 100.000f;
    #endif

    this->addChildAtPosition(
        nameLabel,
        Anchor::Left,
        { 26, 4 }
    );

    auto idLabel = CCLabelBMFont::create(
        mod->getID().c_str(),
        "bigFont.fnt"
    );

    idLabel->setScale(0.2f);
    idLabel->setAnchorPoint({0.0f, 0.5f});
    idLabel->setOpacity(128);

    this->addChildAtPosition(
        idLabel,
        Anchor::Left,
        { 26, -6 }
    );

    auto menu = CCMenu::create();
    menu->setAnchorPoint({1.0f, 0.5f});
    menu->setContentSize({ padding_left, size.height});

    #ifdef GEODE_IS_ARM_MAC
    auto toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(ModItem::onToggle), .6f
    );
    
    toggle->toggle(mod->isLoggingEnabled());

    menu->addChildAtPosition(
        toggle,
        Anchor::Right,
        { 0, 0 }
    );
    #else
    auto& unlogData = UnlogData::data->operator[](mod->getID());

    for (int i = 3; i >= 0; i--) {
        auto toggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(ModItem::onToggle), .6f
        );
        toggle->setTag(i);
        toggle->toggle(unlogData.get(i));
        menu->addChildAtPosition(
            toggle,
            Anchor::Right,
            { 0, 0 }
        );
    }
    #endif

    auto layout = RowLayout::create();
    layout->setDefaultScaleLimits(0.5f, 0.7f);
    layout->setAxisAlignment(AxisAlignment::End);
    layout->setAxisReverse(true);
    menu->setLayout(layout);

    #ifdef GEODE_IS_ARM_MAC
    constexpr float y = 0.0f;
    #else
    constexpr float y = -3.0f;
    #endif
    this->addChildAtPosition(
        menu,
        Anchor::Right,
        { -3, y }
    );

    nameLabel->limitLabelWidth(menu->getPositionX() - menu->getContentWidth() - nameLabel->getPositionX() - 5, .4f, .01f);

    #ifndef GEODE_IS_ARM_MAC
    constexpr std::array labels = {
        "Debug",
        "Info",
        "Warn",
        "Error"
    };

    for (auto toggle : CCArrayExt<CCMenuItemToggler*>(menu->getChildren())) {
        auto label = CCLabelBMFont::create(labels[toggle->getTag()], "bigFont.fnt");

        label->setScale(0.2f);
        label->setPosition(this->convertToNodeSpace(menu->convertToWorldSpace(toggle->getPosition())) + CCPoint{ 0, 11 });

        this->addChild(label);
    }
    #endif

    return true;
}

void ModItem::onToggle(CCObject* sender){
    auto toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    #ifdef GEODE_IS_ARM_MAC
    m_mod->setLoggingEnabled(toggled);
    Mod::get()->setSavedValue(m_mod->getID(), toggled);
    #else
    UnlogData::data->operator[](m_mod->getID()).get(sender->getTag()) = toggled;
    #endif
}

ModItem* ModItem::create(Mod* mod, CCSize const& size) {
    auto ret = new ModItem();
    if (!ret || !ret->init(mod, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}