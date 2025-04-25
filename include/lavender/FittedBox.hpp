#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    enum class BoxFit {
        // Scale the child to fill the parent.
        Fill,
        // Scale the child to fit within the parent while maintaining aspect ratio.
        Contain,
        // Scale the child to cover the parent while maintaining aspect ratio.
        Cover,
        // Scale the child to fit the width of the parent.
        FitWidth,
        // Scale the child to fit the height of the parent.
        FitHeight,
        // Do not scale the child.
        None,
        // Scale the child down to fit within the parent if necessary.
        ScaleDown,
    };

    namespace impl {
        class FittedLayout : public geode::Layout {
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
                // minSize = 0, 0 maxSize = 200, FLT_MAX
                auto [minSize, maxSize] = utils::getConstraints(in);
                auto const minSideSmall = std::min(minSize.width, minSize.height);
                auto const minSideLarge = std::max(minSize.width, minSize.height);
                auto const maxSideSmall = std::min(maxSize.width, maxSize.height);
                auto const maxSideLarge = std::max(maxSize.width, maxSize.height);
                if (auto child = utils::getChild(in)) {
                    switch (m_fit) {
                        case BoxFit::Fill: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scaleX = maxSize.width == FLT_MAX ? heightScale : widthScale;
                            auto const scaleY = maxSize.height == FLT_MAX ? widthScale : heightScale;

                            child->setScaleX(scaleX);
                            child->setScaleY(scaleY);

                            in->setContentSize(childSize * ccp(scaleX, scaleY));
                            break;
                        }
                        case BoxFit::Contain: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = std::min(
                                maxSize.width == FLT_MAX ? heightScale : widthScale, 
                                maxSize.height == FLT_MAX ? widthScale : heightScale
                            );

                            child->setScale(scale);

                            in->setContentSize(childSize * scale);
                            break;
                        }
                        case BoxFit::Cover: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = std::max(
                                maxSize.width == FLT_MAX ? heightScale : widthScale, 
                                maxSize.height == FLT_MAX ? widthScale : heightScale
                            );

                            child->setScale(scale);

                            auto scaledSize = childSize * scale;
                            scaledSize.width = std::min(scaledSize.width, maxSize.width);
                            scaledSize.height = std::min(scaledSize.height, maxSize.height);
                            in->setContentSize(scaledSize);
                            break;
                        }
                        case BoxFit::FitWidth: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = maxSize.width == FLT_MAX ? heightScale : widthScale;

                            child->setScale(scale);

                            in->setContentSize(childSize * scale);
                            break;
                        }
                        case BoxFit::FitHeight: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = maxSize.height == FLT_MAX ? widthScale : heightScale;

                            child->setScale(scale);

                            in->setContentSize(childSize * scale);
                            break;
                        }
                        case BoxFit::None: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = std::min(
                                maxSize.width == FLT_MAX ? heightScale : widthScale, 
                                maxSize.height == FLT_MAX ? widthScale : heightScale
                            );

                            in->setContentSize(childSize * scale);
                            break;
                        }
                        case BoxFit::ScaleDown: {
                            utils::setConstraints(child, {0.f, 0.f}, {FLT_MAX, FLT_MAX});
                            child->updateLayout();

                            auto const childSize = child->getContentSize();
                            auto const widthScale = maxSize.width / childSize.width;
                            auto const heightScale = maxSize.height / childSize.height;
                            auto const scale = std::min(
                                maxSize.width == FLT_MAX ? heightScale : widthScale, 
                                maxSize.height == FLT_MAX ? widthScale : heightScale
                            );

                            if (scale < 1.f) {
                                child->setScale(scale);
                                in->setContentSize(childSize * scale);
                            }
                            else {
                                in->setContentSize(childSize);
                            }
                            break;
                        }
                    }

                    auto const childSize = child->getContentSize() * child->getScale();
                    auto const stepSize = (in->getContentSize() - childSize) / 2.f; // single step to change alignment by 1

                    auto const center = in->getContentSize() / 2.f;
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
        
        BoxFit fit = BoxFit::Contain;
        Alignment alignment = Alignment::Center;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            (void)utils::applyChild(this, node);
            node->setLayout(impl::FittedLayout::create(this->fit, this->alignment));

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}