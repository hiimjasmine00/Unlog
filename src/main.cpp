#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModListPopup.hpp"
#include "UnlogData.hpp"

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

void vlogImplHook(Severity severity, Mod* mod, fmt::string_view format, fmt::format_args args) {
	if (UnlogData::data[mod->getID()][severity]) {
		log::vlogImpl(severity, mod, format, args);
	}
}

$on_mod(Loaded) {
	Mod* mod = Mod::get();
	if (!mod->getSaveContainer().contains("unlog-data")) {
		for (Mod* mod : Loader::get()->getAllMods()) {
			UnlogData::data[mod->getID()] = mod->getSavedValue(mod->getID(), true);
		}
		mod->setSavedValue("unlog-data", UnlogData::data);
	}
	else {
		UnlogData::data = mod->getSavedValue<UnlogData::Map>("unlog-data");
	}

	(void) mod->hook(
		reinterpret_cast<void*>(addresser::getNonVirtual(&log::vlogImpl)),
		&vlogImplHook,
		"geode::log::vlogImpl"
	);
}