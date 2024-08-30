#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class AlignmentLayout : public cocos2d::Layout {
        public:
            Alignment m_alignment;

            static AlignmentLayout* create(Alignment alignment) {
                auto ret = new (std::nothrow) AlignmentLayout();
                if (ret) {
                    ret->m_alignment = alignment;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                auto child = utils::getChild(in);
                if (child) {
                    utils::setConstraints(child, ccp(0, 0), maxSize);
                    child->updateLayout();

                    auto const childSize = child->getContentSize();
                    auto const stepSize = (maxSize - childSize) / 2.f; // single step to change alignment by 1

                    auto const center = maxSize / 2.f;
                    auto const offset = cocos2d::CCSize(
                        m_alignment.x * stepSize.width,
                        m_alignment.y * stepSize.height
                    );

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(center + offset);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                }
                in->setContentSize(maxSize);
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

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                node->setLayout(
                    impl::AlignmentLayout::create(this->alignment)
                );
            }

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };

    struct Center : public BaseInitializer<Center> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                node->setLayout(
                    impl::AlignmentLayout::create(Alignment::Center)
                );
            }

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}