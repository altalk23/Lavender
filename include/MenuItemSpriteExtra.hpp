#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct MenuItemSpriteExtra : public BaseInitializer<MenuItemSpriteExtra> {
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILD();
        UICONSTRUCTOR_ADD_COPY_SIZE();

        std::function<void(cocos2d::CCObject*)> callback;

        cocos2d::CCNode* construct() {
            if (this->child == nullptr) {
                delete this;
                return nullptr;
            }

            auto normal = this->child->construct();
            auto node = geode::cocos::CCMenuItemExt::createSpriteExtra(normal, callback);

            utils::applyID(this, node);
            utils::applyCopySize(this, node, normal);

            utils::updateCopySize(this, node);

            delete this;
            return node;
        }
    };
}