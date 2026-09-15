#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/OverlayManager.hpp>
#include <Geode/utils/random.hpp>

using namespace geode::prelude;

namespace {
   
    // Struct used for tracking which characters are enabled and have randomization enabled
    struct CharacterConfig {
        std::string name;
        std::string settingKey;
        CCSprite* node = nullptr;
        bool isRandomized = false;
    };

    // Randomize sprite each time you reenable the setting
    void setRandom(CCSprite* sprite) {
        if (!sprite) return;
        
        auto winSize = CCDirector::sharedDirector()->getWinSize();
            sprite->setPosition({random::generate<float>(0.f, winSize.width), random::generate<float>(0, winSize.height)});
            sprite->setRotation(random::generate<float>(0.f,360.f));
            sprite->setScale(random::generate<float>(0.1f, 5.0f));
            sprite->setOpacity(random::generate(1,255));
    }
    
    // Add Mr. Washee Washee node
    void createWashee(bool value) {
        if (value) {
            // Prevent duplicates
            if (OverlayManager::get()->getChildByID("mrwasheewashee"_spr)) return;
            
            // Create sprite
            auto washee = CCSprite::create("mrwasheewashee.png"_spr);
            if (!washee) return;
    
            auto winSize = CCDirector::sharedDirector()->getWinSize();

            // Randomzie if setting enabled
            bool randomize = Mod::get()->getSettingValue<bool>("randomize-washee");
            if (randomize) {
                setRandom(washee);
            } else {
                // Default position if not enabled
                washee->setPosition({ (winSize.width * 3.0f) / 4.0f, winSize.height / 2.0f });
            }
            
            // Set ID
            washee->setID("mrwasheewashee"_spr);
    
            // Add to screen
            OverlayManager::get()->addChild(washee);
        } else {
            // Get node and remove from screen when setting disabled
            auto washeeNode = OverlayManager::get()->getChildByID("mrwasheewashee"_spr);
            if (washeeNode) {
                washeeNode->removeFromParentAndCleanup(true);
            }
        }
    }

    // Draws Moe on screen. Same logic as Mr. Washee Washee
    void createMoe(bool value) {
        if (value) {
            if (OverlayManager::get()->getChildByID("moe"_spr)) return;
            
            auto moe = CCSprite::create("moe.png"_spr);
            if (!moe) return;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            bool randomize = Mod::get()->getSettingValue<bool>("randomize-moe");
            if (randomize) {
                setRandom(moe);
            } else {
                moe->setPosition({ (winSize.width) / 4.0f, winSize.height / 2.0f });
            }
            moe->setID("moe"_spr);
    
            OverlayManager::get()->addChild(moe);
        } else {
            auto moeNode = OverlayManager::get()->getChildByID("moe"_spr);
            if (moeNode) {
                moeNode->removeFromParentAndCleanup(true);
            }
        }
    }

    // Draws Roger on screen
    void createRoger(bool value) {
        if (value) {
            if (OverlayManager::get()->getChildByID("roger"_spr)) return;

            auto roger = CCSprite::create("rogersmith.png"_spr);
            if (!roger) return;

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            bool randomize = Mod::get()->getSettingValue<bool>("randomize-roger");
            if (randomize) {
                setRandom(roger);
            } else {
                roger->setPosition({ winSize.width / 2.0f, winSize.height / 2.0f });
            }
            roger->setID("roger"_spr);

            OverlayManager::get()->addChild(roger);
        } else {
            auto rogerNode = OverlayManager::get()->getChildByID("roger"_spr);
            if (rogerNode) {
                rogerNode->removeFromParentAndCleanup(true);
            }
        }
    }
}

// Adds randomize button to main menu
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
			return false;
		}

        auto randomizeBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), this, menu_selector(MyMenuLayer::onRandomBtn));

        auto menu = this->getChildByID("right-side-menu");

        randomizeBtn->setID("randomize-characters"_spr);

        if (menu) {
		    menu->addChild(randomizeBtn);
			menu->updateLayout();
		}

        return true;
    }

    // Rerandomize characters every time button is pressed
    void onRandomBtn(CCObject*) {
        
        // Keep track of current characters in the mod
        std::vector<CharacterConfig> all_characters {
            {"mrwasheewashee"_spr, "randomize-washee"},
            {"moe"_spr, "randomize-moe"},
            {"roger"_spr, "randomize-roger"}
        };

        bool atLeastOneEnabled = false;
        bool atLeastOneRandomized = false;

        // Check each character to see if they're enabled and randomized
        for (auto character : all_characters) {
            auto charNode = OverlayManager::get()->getChildByID(character.name);
            if (charNode) {
                character.node = typeinfo_cast<CCSprite*>(charNode);
                
                // If the node exists, the character is enabled
                if (character.node) {
                    atLeastOneEnabled = true;
                    character.isRandomized = Mod::get()->getSettingValue<bool>(character.settingKey);
                    
                    // It only randomizes characters that have it enabled
                    // I.e. if Mr. Washee Washee and Moe are enabled, but only Washee Washee has randomization enabled,
                    // Pressing the button only affects Mr. Washee Washee. Moe stays in his default position
                    if (character.isRandomized) {
                        atLeastOneRandomized = true;
                        setRandom(character.node);
                    }
                }
            }
        }

        // Present popup if no characters are enabled
        if (!atLeastOneEnabled) {
            FLAlertLayer::create("No Characters Enabled", 
                "Please enable at least one character in mod settings", "OK")->show();
        }

        // Present popup if characters are enabled but none are randomized
        if (!atLeastOneRandomized) {
            FLAlertLayer::create("No Characters Randomized", 
                "Please enable randomizing for your desired character in the mod settings", "OK")->show();
        }
    }
};

$on_game(Loaded) {
    // Retain settings from last session
    createWashee(Mod::get()->getSettingValue<bool>("enable-washee"));
    createMoe(Mod::get()->getSettingValue<bool>("enable-moe"));
    createRoger(Mod::get()->getSettingValue<bool>("enable-roger"));
    
    // Update Enable Washee setting
    listenForSettingChanges<bool>("enable-washee", [](bool value) {
        createWashee(value);
    });

    // Update Enable Moe setting
    listenForSettingChanges<bool>("enable-moe", [](bool value) {
        createMoe(value);
    });

    // Update Enable Roger setting
    listenForSettingChanges<bool>("enable-roger", [](bool value) {
        createRoger(value);
    });

    // Update Randomize Washee setting
    listenForSettingChanges<bool>("randomize-washee", [](bool value) {
        auto washeeNode = OverlayManager::get()->getChildByID("mrwasheewashee"_spr);
        if (washeeNode) {
            // Get sprite from node
            if (auto washee = typeinfo_cast<CCSprite*>(washeeNode)) {
                if (value) {
                    // Re randomize position
                    setRandom(washee);
                } else {
                    // Return to default
                    auto winSize = CCDirector::sharedDirector()->getWinSize();
                    washee->setPosition({(winSize.width * 3.0f) / 4.0f, winSize.height / 2.0f});
                    washee->setRotation(0);
                    washee->setScale(1);
                    washee->setOpacity(255);
                }
            } 
        } else {
            FLAlertLayer::create("Character not enabled","Please enable Mr. Washee Washee first","OK")->show();
        }
    });

    // Update Randomize Moe setting
    listenForSettingChanges<bool>("randomize-moe", [](bool value) {
        // Same logic as Randomize Washee
        auto moeNode = OverlayManager::get()->getChildByID("moe"_spr);
        if (moeNode) {
            if (auto moe = typeinfo_cast<CCSprite*>(moeNode)) {
                if (value) {
                    setRandom(moe);
                } else {
                    auto winSize = CCDirector::sharedDirector()->getWinSize();
                    moe->setPosition({(winSize.width) / 4.0f, winSize.height / 2.0f});
                    moe->setRotation(0);
                    moe->setScale(1);
                    moe->setOpacity(255);
                }
            } 
        } else {
            FLAlertLayer::create("Character not enabled","Please enable Moe first","OK")->show();
        }
    });

    // Update Randomize Roger setting
    listenForSettingChanges<bool>("randomize-roger", [](bool value) {
        // Same logic as Randomize Washee
        auto rogerNode = OverlayManager::get()->getChildByID("roger"_spr);
        if (rogerNode) {
            if (auto roger = typeinfo_cast<CCSprite*>(rogerNode)) {
                if (value) {
                    setRandom(roger);
                } else {
                    auto winSize = CCDirector::sharedDirector()->getWinSize();
                    roger->setPosition({(winSize.width) / 2.0f, winSize.height / 2.0f});
                    roger->setRotation(0);
                    roger->setScale(1);
                    roger->setOpacity(255);
                }
            } 
        } else {
            FLAlertLayer::create("Character not enabled","Please enable Roger first","OK")->show();
        }
    });
}