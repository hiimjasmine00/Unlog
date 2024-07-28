#include "ModList.hpp"
#include "ModItem.hpp"

std::string toLower(std::string str){
    std::string newStr = str;
    for(auto& c : newStr) {
        c = tolower(c);
    }
    return newStr;
}

bool ModList::init(std::vector<Mod*> mods, CCSize const& size){

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    CCScale9Sprite* bg = CCScale9Sprite::create("square02b_001.png");
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(75);
    bg->setScale(.3f);
    bg->setContentSize(size / bg->getScale());
    m_bg = bg;
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    m_list = ScrollLayer::create({ size.width - 10.f, size.height - 10.f });
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(5.0f)
    );
    this->addChildAtPosition(
        m_list,
        Anchor::Center,
        -m_list->getScaledContentSize() / 2
    );

    CCSize itemSize = {
        m_list->getScaledContentWidth(),
        24.f
    };

    std::sort(mods.begin(), mods.end(), [](const Mod* a, const Mod* b) -> bool {
        return toLower(a->getName()) < toLower(b->getName());
    });

    for (Mod* mod : mods) {
        m_list->m_contentLayer->addChild(ModItem::create(mod, itemSize));
    }

    m_list->m_contentLayer->updateLayout();
    m_list->scrollToTop();

    return true;
}

ModList* ModList::create(std::vector<Mod*> mods, CCSize const& size) {
    auto ret = new ModList();
    if (!ret || !ret->init(mods, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}