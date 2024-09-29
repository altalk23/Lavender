#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class TransformLayout : public cocos2d::Layout {
        public:
            float m_offsetX;
            float m_offsetY;
            float m_anchorX;
            float m_anchorY;

            static TransformLayout* create(float offsetX, float offsetY, float anchorX, float anchorY) {
                auto ret = new (std::nothrow) TransformLayout();
                if (ret) {
                    ret->m_offsetX = offsetX;
                    ret->m_offsetY = offsetY;
                    ret->m_anchorX = anchorX;
                    ret->m_anchorY = anchorY;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                if (auto child = utils::getChild(in)) {
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize());

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(in->getContentSize() / 2.f + ccp(m_offsetX, m_offsetY));
                    child->setAnchorPoint(ccp(m_anchorX, m_anchorY));
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

    struct Transform : public BaseInitializer<Transform> {
        LAVENDER_ADD_ID();

        LAVENDER_ADD_ROTATION();
        LAVENDER_ADD_SCALE();

        std::optional<float> offsetX;
        std::optional<float> offsetY;
        std::optional<cocos2d::CCPoint> offset;

        std::optional<float> anchorX;
        std::optional<float> anchorY;
        std::optional<cocos2d::CCPoint> anchor;

        LAVENDER_ADD_CHILD();
    
        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            (void)utils::applyChild(this, node);

            utils::applyID(this, node);

            utils::applyScale(this, node);
            utils::applyRotation(this, node);

            

            utils::applySingleConstrainedLayout(this, node);

            auto wrapper = utils::generateWrapper(node);
            utils::applySingleConstrainedLayout(this, wrapper);

            cocos2d::CCPoint off = {0, 0};
            if (offset.has_value()) {
                off = offset.value();
            }
            else {
                if (offsetX.has_value()) {
                    off.x = offsetX.value();
                }
                if (offsetY.has_value()) {
                    off.y = offsetY.value();
                }
            }

            cocos2d::CCPoint anc = {0.5f, 0.5f};
            if (anchor.has_value()) {
                anc = anchor.value();
            }
            else {
                if (anchorX.has_value()) {
                    anc.x = anchorX.value();
                }
                if (anchorY.has_value()) {
                    anc.y = anchorY.value();
                }
            }

            wrapper->setLayout(impl::TransformLayout::create(off.x, off.y, anc.x, anc.y));

            delete this;
            return wrapper;
        }
    };
}