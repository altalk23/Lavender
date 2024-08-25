#pragma once

#include <Geode/Geode.hpp>

#include "Utils.hpp"

namespace ui {
    namespace impl {
        class ConstrainedObject : public cocos2d::CCObject {
        public:
            cocos2d::CCSize m_minSize;
            cocos2d::CCSize m_maxSize;

            static ConstrainedObject* create(cocos2d::CCSize minSize, cocos2d::CCSize maxSize) {
                auto ret = new (std::nothrow) ConstrainedObject();
                if (ret) {
                    ret->m_minSize = minSize;
                    ret->m_maxSize = maxSize;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }
        };
    }
}