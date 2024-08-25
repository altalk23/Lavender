#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct EdgeInsets {
        float top = 0.f;
        float right = 0.f;
        float bottom = 0.f;
        float left = 0.f;

        struct All {
            float all = 0.f;

            operator EdgeInsets() const {
                return { all, all, all, all };
            }
        };

        struct Symmetric {
            float vertical = 0.f;
            float horizontal = 0.f;

            operator EdgeInsets() const {
                return { vertical, horizontal, vertical, horizontal };
            }
        };
    };

    namespace impl {
        class ContainerLayout : public cocos2d::Layout {
        public:
            cocos2d::CCNode* m_child;
            EdgeInsets m_padding;

            static ContainerLayout* create(cocos2d::CCNode* child, EdgeInsets padding) {
                auto ret = new ContainerLayout();
                ret->m_child = child;
                ret->m_padding = padding;
                ret->autorelease();
                return ret;
            }
            virtual ~ContainerLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                auto const size = in->getContentSize() - cocos2d::CCSize(
                    m_padding.left + m_padding.right,
                    m_padding.top + m_padding.bottom
                );
                auto const center = size / 2.f + cocos2d::CCSize(
                    m_padding.left,
                    m_padding.bottom
                );
                m_child->ignoreAnchorPointForPosition(false);
                m_child->setContentSize(size);
                m_child->setPosition(center);
                m_child->setAnchorPoint(ccp(0.5f, 0.5f));
                m_child->updateLayout();
            }
            
            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct Container : BaseInitializer<Container> {
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILD();
        UICONSTRUCTOR_ADD_SIZE();

        EdgeInsets padding;

        cocos2d::CCNode* construct() {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                node->setLayout(
                    impl::ContainerLayout::create(child, this->padding)
                );
            }

            utils::applyID(this, node);
            utils::applySize(this, node);

            delete this;
            return node;
        }
    };
}