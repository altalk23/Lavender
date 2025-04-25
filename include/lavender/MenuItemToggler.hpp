#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    struct MenuItemToggler : public BaseInitializer<MenuItemToggler> {
        LAVENDER_ADD_ID();

        std::function<void(cocos2d::CCObject*)> callback = +[](cocos2d::CCObject*) {};

        bool toggled = false;
        ui::Base* on = nullptr;
        ui::Base* off = nullptr;
        std::optional<float> standardScale = 1.f;

        cocos2d::CCNode* construct() const {
            CCMenuItemToggler* node;
            if (this->on == nullptr && this->off == nullptr && this->standardScale.has_value()) {
                node = geode::cocos::CCMenuItemExt::createTogglerWithStandardSprites(*this->standardScale, this->callback);
            }
            else if (this->on != nullptr && this->off != nullptr) {
                auto onSprite = this->on->construct();
                auto offSprite = this->off->construct();
                node = geode::cocos::CCMenuItemExt::createToggler(onSprite, offSprite, this->callback);
            }
            else {
                delete this;
                return nullptr;
            }
            if (this->toggled) {
                node->toggle(this->toggled);
            }

            utils::applyID(this, node);
            utils::applySingleConstrainedLayout(this, node);

            delete this;
            return node;
        }
    };
}