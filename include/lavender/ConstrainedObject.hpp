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

            virtual size_t getABIVersion() const {
                return 1;
            }

            virtual cocos2d::CCSize getMinSize() const {
                return m_minSize;
            }

            virtual cocos2d::CCSize getMaxSize() const {
                return m_maxSize;
            }

            virtual void setMinSize(cocos2d::CCSize size) {
                m_minSize = size;
            }

            virtual void setMaxSize(cocos2d::CCSize size) {
                m_maxSize = size;
            }
        };
    }
}