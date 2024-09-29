#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModListPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCLayer.hpp>

class ModsLayer : public CCNode {};

class $modify(ModsLayerExt, CCLayer) {	
    bool init() {
    	if (!CCLayer::init()) return false;
		if (typeinfo_cast<ModsLayer*>(this)) {
			queueInMainThread([this] {
				bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");
				if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(getChildByID("actions-menu"))) {

					CCSprite* spr = CircleButtonSprite::createWithSprite(
						"terminal.png"_spr, .85f,
						(geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
					);
					spr->setScale(0.8f);

					CCMenuItemSpriteExtra* logsBtn = CCMenuItemExt::createSpriteExtra(
						spr, [](CCNode*) {
							ModListPopup::create()->show();
						}
					);
					logsBtn->setID("mods-logs-button");
					actionsMenu->addChild(logsBtn);
					actionsMenu->updateLayout();
				}
			});
		}
		return true;
	}
};

$on_mod(Loaded) {
	for(Mod* mod : Loader::get()->getAllMods()){
		auto value = Mod::get()->getSavedValue<bool>(mod->getID(), true);
		mod->setLoggingEnabled(value);
	}
}