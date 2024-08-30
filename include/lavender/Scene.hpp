#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Scene : public BaseInitializer<Scene> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCScene::create();

            node->setAnchorPoint(ccp(0, 0));

            (void)utils::applyChild(this, node);
            utils::applySingleConstrainedLayout(this, node);

            auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            node->setUserObject("constrain"_spr, impl::ConstrainedObject::create(ccp(0, 0), winSize));

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}