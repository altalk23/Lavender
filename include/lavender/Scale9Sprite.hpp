#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    struct Scale9Sprite : BaseInitializer<Scale9Sprite> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_SCALE();
        LAVENDER_ADD_SIZE();
        LAVENDER_ADD_COLOR4();
        LAVENDER_ADD_CHILD();

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

            utils::applyID(this, node);
            utils::applyScale(this, node);
            if (this->width.has_value()) {
                node->setContentWidth(this->width.value() / node->getScaleX());
            }
            if (this->height.has_value()) {
                node->setContentHeight(this->height.value() / node->getScaleY());
            }
            if (this->size.has_value()) {
                node->setContentSize(this->size.value() / node->getScale());
            }
            utils::applyColor4(this, node);

            utils::applySizedConstrainedLayout(this, node, false);

            auto wrapper = utils::generateWrapper(node);
            auto child = utils::applyChild(this, wrapper);

            utils::applyPrimarySizedConstrainedLayout(this, wrapper, child);

            delete this;
            return wrapper;
        }
    };
}