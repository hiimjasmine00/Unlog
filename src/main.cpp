#include <Geode/Geode.hpp>
#include "ModListPopup.hpp"
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

class $modify(MyCCDirector, CCDirector) {

	void onModLogs(CCObject* sender) {
		ModListPopup::create()->show();
	}

	bool replaceScene(CCScene* scene) {
		if (!CCDirector::replaceScene(scene)) return false;

		if(scene->getChildrenCount() == 0) return true;

		bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

		if(CCNode* node = getChildOfType<CCNode>(scene, 0)){
			if(node->getID() == "ModsLayer"){
				if(CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(node->getChildByID("actions-menu"))){

					CCSprite* spr = CircleButtonSprite::createWithSprite(
						"terminal.png"_spr, .85f,
						(geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
					);
					spr->setScale(0.8f);

					CCMenuItemSpriteExtra* logsBtn = CCMenuItemSpriteExtra::create(spr, node, menu_selector(MyCCDirector::onModLogs));
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