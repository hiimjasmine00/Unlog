#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModListPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCDirector.hpp>
class $modify(MyCCDirector, CCDirector) {

	bool replaceScene(CCScene* scene) {
		if (!CCDirector::replaceScene(scene)) return false;
		if(scene->getChildrenCount() == 0) return true;

		bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");
		if(CCNode* node = getChildOfType<CCNode>(scene, 0)){
			if(node->getID() == "ModsLayer"){
				if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(node->getChildByID("actions-menu"))) {

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
			}
		}
		return true;
	}
};

$execute {
	for(Mod* mod : Loader::get()->getAllMods()){
		auto value = Mod::get()->getSavedValue<bool>(mod->getID(), true);
		mod->setLoggingEnabled(value);
	}
}