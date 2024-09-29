#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "LayerColor.hpp"
#include "Scale9Sprite.hpp"
#include "Sprite.hpp"
#include "Stack.hpp"
#include "Menu.hpp"
#include "Container.hpp"
#include "Action.hpp"

namespace ui {
    enum class PopupAnimation {
        None,
        Scale,
    };

    struct Popup : public BaseInitializer<Popup> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_SIZE();
        LAVENDER_ADD_CHILD();
        Base* outer = nullptr;

        PopupAnimation animation = PopupAnimation::Scale;

        std::optional<std::string> bgFileName = "GJ_square01.png";
        std::optional<std::string> bgFrameName;
        std::optional<std::string> bgId;

        std::optional<std::string> closeFileName;
        std::optional<std::string> closeFrameName = "GJ_closeBtn_001.png";
        std::optional<std::string> closeId;

        cocos2d::CCNode* construct() const {
            cocos2d::CCAction* action = nullptr;
            float tscale = 1.f;

            switch (animation) {
                case PopupAnimation::Scale:
                    action = cocos2d::CCEaseElasticOut::create(
                        cocos2d::CCScaleTo::create(0.5f, 1.f),
                        0.6f
                    );
                    tscale = 0.1f;
                    break;
                default:
                    break;
            };

            cocos2d::CCNode* root = nullptr;
            cocos2d::CCNode* stack = nullptr;
            cocos2d::CCNode* outerStack = nullptr;

            auto gen = new LayerColor {
                .id = this->id,
                .store = &root,
                .color = cocos2d::ccc4(0, 0, 0, 1),
                .keyBackClicked = [](auto* self) {
                    self->removeFromParentAndCleanup(true);
                },
                .onTouchBegan = [](auto* self, cocos2d::CCTouch*, cocos2d::CCEvent*) {
                    return true;
                },
                .init = [](auto* self) {
                    cocos2d::CCTouchDispatcher::get()->registerForcePrio(self, 2);
                },
                .destruct = [](auto* self) {
                    cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(self);
                },
                .registerWithTouchDispatcher = [](auto* self) {
                    cocos2d::CCTouchDispatcher::get()->addTargetedDelegate(self, -500, true);
                },
                .child = new Stack {
                    .store = &outerStack,
                    .children = {
                        new Scale9Sprite {
                            .id = this->bgId,
                            .fileName = this->bgFrameName.has_value() ? std::nullopt : this->bgFileName,
                            .frameName = this->bgFrameName,
                            .size = this->size,
                        },
                        new Container {
                            .size = this->size,
                            .child = new Stack {
                                .store = &stack,
                            },
                        },
                        this->outer,
                    },
                },
            };
            auto node = gen->construct();
            node->runAction(cocos2d::CCFadeTo::create(0.14f, 105));

            outerStack->setScale(tscale);
            outerStack->runAction(action);

            auto menuGen = new Menu {
                .child = new Align {
                    .alignment = Alignment::TopLeft,
                    .child = new Transform {
                        .offset = ccp(3.f, -3.f),
                        .anchor = ccp(1.f, 0.f),
                        .child = new MenuItemSpriteExtra {
                            .callback = [=](auto* self) {
                                root->removeFromParentAndCleanup(true);
                            },
                            .child = new Sprite {
                                .id = this->closeId,
                                .fileName = this->closeFileName,
                                .frameName = this->closeFileName.has_value() ? std::nullopt : this->closeFrameName,
                                .scale = 0.8f,
                            },
                        },
                    },
                },
            };
            stack->addChild(menuGen->construct());
            if (this->child) {
                stack->addChild(this->child->construct());
            }

            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            node->setUserObject("constrain"_spr, impl::ConstrainedObject::create(ccp(0, 0), winSize));
            
            auto highestZ = cocos2d::CCScene::get()->getHighestChildZ();
            node->setZOrder(highestZ + 1);
            cocos2d::CCScene::get()->addChild(node);

            delete this;
            return node;
        }
    };
}