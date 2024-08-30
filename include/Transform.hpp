#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "ConstrainedLayout.hpp"

namespace ui {
    struct Transform : public BaseInitializer<Transform> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        LAVENDER_ADD_ROTATION();
        LAVENDER_ADD_SCALE();

        std::optional<float> offsetX;
        std::optional<float> offsetY;
        std::optional<cocos2d::CCPoint> offset;
    
        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            (void)utils::applyChild(this, node);

            utils::applyID(this, node);

            utils::applyScale(this, node);
            utils::applyRotation(this, node);

            if (offsetX.has_value()) {
                node->setPositionX(node->getContentWidth() / 2 + offsetX.value());
            }
            if (offsetY.has_value()) {
                node->setPositionY(node->getContentHeight() / 2 + offsetY.value());
            }
            if (offset.has_value()) {
                node->setPosition(node->getContentSize() / 2 + offset.value());
            }

            auto wrapper = utils::generateWrapper(node);
            utils::applySingleConstrainedLayout(this, wrapper);

            delete this;
            return wrapper;
        }
    };
}