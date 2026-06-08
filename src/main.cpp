#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

class $modify(GlobalOverlay, CCDirector) {
    void drawScene() {

        // I know I'm not supposed to call this manually but when I removed it the game wouldn't load
        CCDirector::drawScene();

        // Get current scene
        auto currScene = this->getRunningScene();
        if (!currScene) {
            return;
        }

        // All mod settings
        bool isWasheeEnabled = Mod::get()->getSettingValue<bool>("enable-washee");
        bool isMoeEnabled = Mod::get()->getSettingValue<bool>("enable-moe");
        bool isRandomizingWashee = Mod::get()->getSettingValue<bool>("randomize-washee");
        bool isRandomizingMoe = Mod::get()->getSettingValue<bool>("randomize-moe");

        // Check if Mr. Washee Washee is enabled to show up on screen
        if (isWasheeEnabled) {
            
            // Draws Mr. Washee Washee
            if (!currScene->getChildByID("mrwasheewashee")) {
                auto winSize = CCDirector::sharedDirector()->getWinSize();

                // Get sprite and set up node
                auto washee = CCSprite::create("mrwasheewashee.png"_spr);

                if (washee) {
                    washee->setID("mrwasheewashee");

                    // Randomize position, rotation, scale, and opacity if the setting is enabled
                    if (isRandomizingWashee) {
                        washee->setPosition({ CCRANDOM_0_1() * winSize.width, CCRANDOM_0_1() * winSize.height });
                        washee->setRotation(CCRANDOM_0_1() * 360.0f);
                        washee->setScale(0.5f + CCRANDOM_0_1());
                        washee->setOpacity(CCRANDOM_0_1() * 255);
                    } else {
                        
                        // Default position
                        washee->setPosition({ (winSize.width * 3.0f) / 4.0f, winSize.height / 2.0f });
                    }

                    // Add him to scene. High Z-order to put it above everything else
                    currScene->addChild(washee, 9999);
                } else {
                    log::error("Failed to load sprite");
                }
            }
        } else {
            auto washee = currScene->getChildByID("mrwasheewashee");
            
            // Removes Mr. Washee Washee if setting is disabled and is he's still there
            if (washee) {
                washee->removeFromParentAndCleanup(true);
            }
        }

        // Adds Moe if enabled. Same logic as Mr. Washee Washee
        if (isMoeEnabled) {
            
            // Draws Moe
            if (!currScene->getChildByID("moe")) {
                auto winSize = CCDirector::sharedDirector()->getWinSize();

                // Get sprite and set up node
                auto moe = CCSprite::create("moe.png"_spr);
                if (moe) {
                    moe->setID("moe");

                    // Randomize position, rotation, scale, and opacity if the setting is enabled
                    if (isRandomizingMoe) {
                        moe->setPosition({ CCRANDOM_0_1() * winSize.width, CCRANDOM_0_1() * winSize.height });
                        moe->setRotation(CCRANDOM_0_1() * 360.0f);
                        moe->setScale(0.5f + CCRANDOM_0_1());
                        moe->setOpacity(CCRANDOM_0_1() * 255);
                    } else {
                        moe->setPosition({ winSize.width / 4.0f, winSize.height / 2.0f });
                    }

                    // Add Moe to scene. High Z-order to put it above everything else
                    currScene->addChild(moe, 9999);
                } else {
                    log::error("Failed to load sprite");
                }
            }
        } else {
            auto moe = currScene->getChildByID("moe");
            if (moe) {
                
                // Removes Moe if setting is disabled and he's still there
                moe->removeFromParentAndCleanup(true);
            }
        }
    }
};