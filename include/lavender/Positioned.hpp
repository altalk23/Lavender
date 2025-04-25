#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class PositionedLayout : public geode::Layout {
        public:
            std::optional<cocos2d::CCPoint> m_offset;

            static PositionedLayout* create(
                std::optional<cocos2d::CCPoint> offset
            ) {
                auto ret = new (std::nothrow) PositionedLayout();
                if (ret) {
                    ret->m_offset = offset;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }
            virtual ~PositionedLayout() = default;
        
            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                if (m_offset.has_value()) {
                    minSize -= m_offset.value();
                    maxSize -= m_offset.value();
                }

                if (auto child = utils::getChild(in); child) {
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize());

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(in->getContentSize() / 2.f + m_offset.value_or(ccp(0, 0)));
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                }
                else {
                    in->setContentSize(maxSize);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct Positioned : public BaseInitializer<Positioned> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        std::optional<cocos2d::CCPoint> offset;

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();
            utils::applyID(this, node);
            utils::applyChild(this, node);

            node->setLayout(
                impl::PositionedLayout::create(this->offset)
            );

            delete this;
            return node;
        }
    };
}