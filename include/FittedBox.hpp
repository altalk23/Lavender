#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    enum class BoxFit {
        Fill,
        Contain,
        Cover,
        FitWidth,
        FitHeight,
        None,
        ScaleDown,
    };

    namespace impl {
        class FittedLayout : public cocos2d::Layout {
        public:
            BoxFit m_fit;
            Alignment m_alignment;

            static FittedLayout* create(BoxFit fit, Alignment alignment) {
                auto ret = new (std::nothrow) FittedLayout();
                if (ret) {
                    ret->m_fit = fit;
                    ret->m_alignment = alignment;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                auto const minSideSmall = std::min(minSize.width, minSize.height);
                auto const minSideLarge = std::max(minSize.width, minSize.height);
                auto const maxSideSmall = std::min(maxSize.width, maxSize.height);
                auto const maxSideLarge = std::max(maxSize.width, maxSize.height);
                if (auto child = utils::getChild(in)) {
                    switch (m_fit) {
                        case BoxFit::Fill: {
                            utils::setConstraints(child, maxSize, maxSize);
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scaleX = maxSize.width / childSize.width;
                            auto const scaleY = maxSize.height / childSize.height;

                            child->setScaleX(scaleX);
                            child->setScaleY(scaleY);

                            break;
                        }
                        case BoxFit::Contain: {
                            utils::setConstraints(child, ccp(minSideLarge, minSideLarge), ccp(maxSideSmall, maxSideSmall));
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scale = std::min(maxSize.width / childSize.width, maxSize.height / childSize.height);

                            child->setScale(scale);

                            break;
                        }
                        case BoxFit::Cover: {
                            utils::setConstraints(child, ccp(minSideSmall, minSideSmall), ccp(maxSideLarge, maxSideLarge));
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scale = std::max(maxSize.width / childSize.width, maxSize.height / childSize.height);

                            child->setScale(scale);
                            break;
                        }
                        case BoxFit::FitWidth: {
                            utils::setConstraints(child, ccp(minSize.width, minSize.width), ccp(maxSize.width, maxSize.width));
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scale = maxSize.width / childSize.width;

                            child->setScale(scale);
                        }
                        case BoxFit::FitHeight: {
                            utils::setConstraints(child, ccp(minSize.height, minSize.height), ccp(maxSize.height, maxSize.height));
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scale = maxSize.height / childSize.height;

                            child->setScale(scale);
                        }
                        case BoxFit::None:
                            utils::setConstraints(child, minSize, maxSize);
                            child->updateLayout();
                            break;
                        case BoxFit::ScaleDown: {
                            utils::setConstraints(child, minSize, maxSize);
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const scale = std::min(maxSize.width / childSize.width, maxSize.height / childSize.height);
                            if (scale < 1.f) {
                                child->setScale(scale);
                            }
                            break;
                        }
                    }

                    in->setContentSize(maxSize);

                    auto const childSize = child->getContentSize() * child->getScale();
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
                else {
                    in->setContentSize(minSize);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct FittedBox : BaseInitializer<FittedBox> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
        BoxFit fit = BoxFit::Contain;
        Alignment alignment = Alignment::Center;

        cocos2d::CCNode* construct() {
            auto node = cocos2d::CCNode::create();

            (void)utils::applyChild(this, node);
            node->setLayout(impl::FittedLayout::create(this->fit, this->alignment));

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}