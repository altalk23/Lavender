#pragma once

#include <Geode/Geode.hpp>

namespace ui {
    namespace impl {
        class CopySizeLayout : public cocos2d::Layout {
        public:
            cocos2d::CCNode* m_child;

            static CopySizeLayout* create(cocos2d::CCNode* child) {
                auto ret = new (std::nothrow) CopySizeLayout();
                if (ret) {
                    ret->m_child = child;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }
            virtual ~CopySizeLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                m_child->ignoreAnchorPointForPosition(false);
                m_child->setContentSize(in->getContentSize());
                m_child->setPosition(in->getContentSize() / 2.f);
                m_child->setAnchorPoint(ccp(0.5f, 0.5f));
                m_child->updateLayout();
            }
            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }
}