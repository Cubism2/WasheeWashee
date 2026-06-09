#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/ui/OverlayManager.hpp>
#include <Geode/utils/random.hpp>

using namespace geode::prelude;

namespace {
    
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
}

$on_game(Loaded) {
    // Retain settings from last session
    createWashee(Mod::get()->getSettingValue<bool>("enable-washee"));
    createMoe(Mod::get()->getSettingValue<bool>("enable-moe"));
    
    // Update Enable Washee setting
    listenForSettingChanges<bool>("enable-washee", [](bool value) {
        createWashee(value);
    });

    // Update Enable Moe setting
    listenForSettingChanges<bool>("enable-moe", [](bool value) {
        createMoe(value);
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
        }
    });
}