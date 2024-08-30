#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    struct MenuItemSpriteExtra : public BaseInitializer<MenuItemSpriteExtra> {
        LAVENDER_ADD_ID();

        std::function<void(cocos2d::CCObject*)> callback;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            if (this->child == nullptr) {
                delete this;
                return nullptr;
            }

            auto normal = this->child->construct();
            auto node = geode::cocos::CCMenuItemExt::createSpriteExtra(normal, callback);

            utils::applyID(this, node);
            utils::applySingleConstrainedLayout(this, node);

            delete this;
            return node;
        }
    };
}