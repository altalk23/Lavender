#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Scene : public BaseInitializer<Scene> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() {
            auto node = cocos2d::CCScene::create();

            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            node->setContentSize(winSize);
            node->setAnchorPoint(ccp(0, 0));

            if (auto child = utils::applyChild(this, node)) {
                utils::applyCopySize(this, node, child);
            }
            else {
                delete this;
                return nullptr;
            }

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}