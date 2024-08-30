#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class RotatedLayout : public cocos2d::Layout {
        public:
            int m_quarterTurns = 0;

            static RotatedLayout* create(int quarterTurns) {
                auto ret = new (std::nothrow) RotatedLayout();
                if (ret) {
                    ret->m_quarterTurns = quarterTurns;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                if (auto child = utils::getChild(in)) {
                    if (m_quarterTurns % 2 == 1) {
                        std::swap(minSize.width, minSize.height);
                        std::swap(maxSize.width, maxSize.height);
                    }
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    if (m_quarterTurns % 2 == 1) {
                        in->setContentSize(cocos2d::CCSize(child->getContentSize().height, child->getContentSize().width));
                    }
                    else {
                        in->setContentSize(child->getContentSize());
                    }

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

    struct RotatedBox : BaseInitializer<RotatedBox> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
        int quarterTurns = 0;

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                child->setRotation(this->quarterTurns * 90.f);
            } 
            node->setLayout(impl::RotatedLayout::create(this->quarterTurns));

            utils::applyID(this, node);
            

            delete this;
            return node;
        }
    };
}