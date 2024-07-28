#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModList : public CCNode {

    protected:

        ScrollLayer* m_list;
        CCScale9Sprite* m_bg;

        bool init(std::vector<Mod*> mods, CCSize const& size);
    public:
        static ModList* create(std::vector<Mod*> mods, CCSize const& size);
};