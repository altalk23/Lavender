#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "Expanded.hpp"

namespace ui {
    struct Flexible : public BaseInitializer<Flexible> {
        LAVENDER_ADD_ID();

        size_t flex = 1;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            if (this->flex < 1) {
                delete this;
                return nullptr;
            }

            auto node = impl::Expanded::create(this->flex);

            (void)utils::applyChild(this, node);
            utils::applySingleConstrainedLayout(this, node);

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}