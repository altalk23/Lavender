#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class Expanded : public cocos2d::CCNode {
        public:
            size_t m_flex;
            Axis m_mainAxis;

            static Expanded* create(size_t flex) {
                auto ret = new (std::nothrow) Expanded();
                if (ret && ret->init(flex)) {
                    ret->autorelease();
                    return ret;
                }
                delete ret;
                return nullptr;
            }
            
            bool init(size_t flex) {
                if (!cocos2d::CCNode::init()) {
                    return false;
                }
                
                m_flex = flex;
                return true;
            }
        };

        class ExpandedLayout : public cocos2d::Layout {
        public:
            size_t m_flex;

            static ExpandedLayout* create(size_t flex) {
                auto ret = new (std::nothrow) ExpandedLayout();
                if (ret) {
                    ret->m_flex = flex;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                if (auto expanded = dynamic_cast<Expanded*>(in)) {
                    switch (expanded->m_mainAxis) {
                        case Axis::Horizontal:
                            minSize.width = maxSize.width;
                            break;
                        case Axis::Vertical:
                            minSize.height = maxSize.height;
                            break;
                    }
                }
                if (auto child = utils::getChild(in)) {
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize());

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(in->getContentSize() / 2.f);
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

    struct Expanded : BaseInitializer<Expanded> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();

        size_t flex = 1;

        cocos2d::CCNode* construct() {
            if (this->flex < 1) {
                delete this;
                return nullptr;
            }

            auto node = impl::Expanded::create(this->flex);

            (void)utils::applyChild(this, node);
            node->setLayout(impl::ExpandedLayout::create(this->flex));

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}