#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Scene : public BaseInitializer<Scene> {
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILD();
        UICONSTRUCTOR_ADD_COPY_SIZE();

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

            utils::updateCopySize(this, node);

            delete this;
            return node;
        }
    };
}