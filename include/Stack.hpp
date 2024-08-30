#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    enum class StackFit {
        Loose,
        Expand,
    };

    namespace impl {
        class StackLayout : public cocos2d::Layout {
        public:
            Alignment m_alignment;
            StackFit m_fit;

            static StackLayout* create(Alignment alignment, StackFit fit) {
                auto ret = new (std::nothrow) StackLayout();
                if (ret) {
                    ret->m_alignment = alignment;
                    ret->m_fit = fit;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);

                for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {
                    switch (m_fit) {
                        case StackFit::Loose:
                            utils::setConstraints(child, minSize, maxSize);
                            break;
                        case StackFit::Expand:
                            utils::setConstraints(child, maxSize, maxSize);
                            break;
                    }
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

    struct Stack : BaseInitializer<Stack> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILDREN();
        Alignment alignment = Alignment::Center;
        StackFit fit = StackFit::Loose;

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();
            utils::applyChildren(this, node);

            node->setLayout(impl::StackLayout::create(this->alignment, this->fit));

            utils::applyID(this, node);

            return node;
        }
    };
}