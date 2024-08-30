#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "ConstrainedLayout.hpp"

namespace ui {
    struct Hidden : public BaseInitializer<Hidden> {
        LAVENDER_ADD_ID();

        bool hidden = true;

        LAVENDER_ADD_CHILD();
    
        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            (void)utils::applyChild(this, node);
            utils::applySingleConstrainedLayout(this, node);

            utils::applyID(this, node);

            node->setVisible(!this->hidden);

            delete this;
            return node;
        }
    };
}