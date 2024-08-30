#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    struct Scale9Sprite : BaseInitializer<Scale9Sprite> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
        LAVENDER_ADD_SCALE();
        LAVENDER_ADD_SIZE();
        LAVENDER_ADD_COLOR();

        std::optional<std::string> fileName;
        std::optional<std::string> frameName;

        cocos2d::CCNode* construct() const {
            cocos2d::extension::CCScale9Sprite* node;
            
            if (fileName.has_value()) {
                node = cocos2d::extension::CCScale9Sprite::create(fileName.value().c_str());
            }
            else if (frameName.has_value()) {
                node = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName(frameName.value().c_str());
            }
            else {
                delete this;
                return nullptr;
            }

            (void)utils::applyChild(this, node);
            utils::applySizedConstrainedLayout(this, node);

            utils::applyID(this, node);
            utils::applyScale(this, node);
            utils::applySize(this, node);
            utils::applyColor(this, node);

            delete this;
            return node;
        }
    };
}