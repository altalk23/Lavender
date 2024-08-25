#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Scale9Sprite : BaseInitializer<Scale9Sprite> {
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILD();
        UICONSTRUCTOR_ADD_SCALE();
        UICONSTRUCTOR_ADD_SIZE();
        UICONSTRUCTOR_ADD_COLOR();

        std::optional<std::string> fileName;
        std::optional<std::string> frameName;

        cocos2d::CCNode* construct() {
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

            if (auto child = utils::applyChild(this, node)) {
                utils::applyCopySize(this, node, child);
            }

            utils::applyID(this, node);
            utils::applyScale(this, node);
            utils::applySize(this, node);
            utils::applyColor(this, node);

            delete this;
            return node;
        }
    };
}