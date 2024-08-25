#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Alignment {
        float x = 0.f;
        float y = 0.f;

        static Alignment TopLeft;
        static Alignment TopCenter;
        static Alignment TopRight;
        static Alignment CenterLeft;
        static Alignment Center;
        static Alignment CenterRight;
        static Alignment BottomLeft;
        static Alignment BottomCenter;
        static Alignment BottomRight;
    };

    inline Alignment Alignment::TopLeft = { -1.f, 1.f };
    inline Alignment Alignment::TopCenter = { 0.f, 1.f };
    inline Alignment Alignment::TopRight = { 1.f, 1.f };
    inline Alignment Alignment::CenterLeft = { -1.f, 0.f };
    inline Alignment Alignment::Center = { 0.f, 0.f };
    inline Alignment Alignment::CenterRight = { 1.f, 0.f };
    inline Alignment Alignment::BottomLeft = { -1.f, -1.f };
    inline Alignment Alignment::BottomCenter = { 0.f, -1.f };
    inline Alignment Alignment::BottomRight = { 1.f, -1.f };

    namespace impl {
        class AlignmentLayout : public cocos2d::Layout {
        public:
            cocos2d::CCNode* m_child;
            Alignment m_alignment;

            static AlignmentLayout* create(cocos2d::CCNode* child, Alignment alignment) {
                auto ret = new AlignmentLayout();
                ret->m_child = child;
                ret->m_alignment = alignment;
                ret->autorelease();
                return ret;
            }
            virtual ~AlignmentLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                auto const availableSize = in->getContentSize();
                if (availableSize.width < m_child->getContentWidth()) {
                    m_child->setContentWidth(availableSize.width);
                }
                if (availableSize.height < m_child->getContentHeight()) {
                    m_child->setContentHeight(availableSize.height);
                }
                auto const childSize = m_child->getContentSize();
                auto const stepSize = (availableSize - childSize) / 2.f; // single step to change alignment by 1

                auto const center = availableSize / 2.f;
                auto const offset = cocos2d::CCSize(
                    m_alignment.x * stepSize.width,
                    m_alignment.y * stepSize.height
                );

                m_child->ignoreAnchorPointForPosition(false);
                m_child->setPosition(center + offset);
                m_child->setAnchorPoint(ccp(0.5f, 0.5f));
                m_child->updateLayout();
            }
            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct Align : public BaseInitializer<Align> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        Alignment alignment = Alignment::Center;

        cocos2d::CCNode* construct() {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                node->setLayout(
                    impl::AlignmentLayout::create(child, this->alignment)
                );
            }

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}