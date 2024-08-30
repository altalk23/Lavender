#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class ScrollLayerLayout : public cocos2d::Layout {
        public:
            Axis m_axis;

            static ScrollLayerLayout* create(Axis axis) {
                auto ret = new (std::nothrow) ScrollLayerLayout();
                if (ret) {
                    ret->m_axis = axis;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                auto emulatedMaxSize = maxSize;
                switch (m_axis) {
                    case Axis::Horizontal:
                        emulatedMaxSize.width = FLT_MAX;
                        break;
                    case Axis::Vertical:
                        emulatedMaxSize.height = FLT_MAX;
                        break;
                }

                if (auto scrollLayer = geode::cast::typeinfo_cast<geode::ScrollLayer*>(in)) {
                    auto child = scrollLayer->m_contentLayer;

                    utils::setConstraints(child, minSize, emulatedMaxSize);
                    child->updateLayout();

                    switch (m_axis) {
                        case Axis::Horizontal:
                            in->setContentHeight(std::max(child->getContentHeight(), minSize.height));
                            in->setContentWidth(maxSize.width);
                            child->setPositionY(in->getContentHeight() / 2);
                            break;
                        case Axis::Vertical:
                            in->setContentWidth(std::max(child->getContentWidth(), minSize.width));
                            in->setContentHeight(maxSize.height);
                            child->setPositionX(in->getContentWidth() / 2);
                            break;
                    }

                    child->ignoreAnchorPointForPosition(false);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                    scrollLayer->scrollToTop();
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

    struct ScrollLayer : public BaseInitializer<ScrollLayer> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        Axis axis = Axis::Vertical;
    
        cocos2d::CCNode* construct() const {
            auto node = geode::ScrollLayer::create(ccp(0, 0), true, this->axis == Axis::Vertical);

            (void)utils::applyChild(this, node->m_contentLayer);
            node->setLayout(impl::ScrollLayerLayout::create(this->axis));

            utils::applyID(this, node);

            utils::applySingleConstrainedLayout(this, node->m_contentLayer);

            delete this;
            return node;
        }
    };
}