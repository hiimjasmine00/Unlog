#include "ModItem.hpp"
#include <Geode/utils/ColorProvider.hpp>

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

    auto nameLabel = CCLabelBMFont::create(
        mod->getName().c_str(),
        "bigFont.fnt"
    );

    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(ModItem::onToggle), .6f
    );
    
    m_toggle->toggle(mod->isLoggingEnabled());

    constexpr float paddings = 30.0f;
    float calc = size.width - paddings;
    nameLabel->setWidth(calc);
    nameLabel->setScale(0.4f);
    nameLabel->setAnchorPoint({0.0f, 0.5f});

    this->addChildAtPosition(
        nameLabel,
        Anchor::Left,
        {5, 4}
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
        {5, -6}
    );

    auto menu = CCMenu::create();
    menu->setAnchorPoint({1.0f, 0.5f});
    menu->setContentSize({size.width/2, size.height});
    menu->addChildAtPosition(
        m_toggle,
        Anchor::Right,
        {0, 0}
    );

    auto layout = RowLayout::create();
    layout->setDefaultScaleLimits(0.5f, 0.7f);
    layout->setAxisAlignment(AxisAlignment::End);
    layout->setAxisReverse(true);
    menu->setLayout(layout);

    this->addChildAtPosition(
        menu,
        Anchor::Right,
        {-3, 0}
    );

    return true;
}

void ModItem::onToggle(CCObject* sender){
    m_mod->setLoggingEnabled(!m_toggle->isToggled());
    Mod::get()->setSavedValue<bool>(m_mod->getID(), !m_toggle->isToggled());
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