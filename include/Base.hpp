#pragma once

#include <Geode/Geode.hpp>

namespace ui {
    using ConstructorType = cocos2d::CCNode*(*)(void const*);

    struct Base {
        ConstructorType constructor;

        cocos2d::CCNode* construct() const {
            return this->constructor(this);
        }

        cocos2d::CCNode* get() {
            auto ret = this->constructor(this);
            ret->updateLayout();
            return ret;
        }
    };

    template <class Parent>
    struct BaseInitializer : public Base {
        BaseInitializer() {
            this->constructor = [](void const* ptr) {
                return static_cast<Parent const*>(ptr)->construct();
            };
        }
    };
}