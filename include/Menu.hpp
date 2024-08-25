#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class MenuWrapper : public cocos2d::CCMenu {
        public:
            static MenuWrapper* create() {
                auto ret = new (std::nothrow) MenuWrapper();
                if (ret && ret->init()) {
                    ret->autorelease();
                    return ret;
                }
                delete ret;
                return nullptr;
            }
            virtual ~MenuWrapper() = default;

            bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
                if (m_eState != cocos2d::kCCMenuStateWaiting || ! m_bVisible) {
                    return false;
                }

                for (auto c = this->m_pParent; c != NULL; c = c->getParent()) {
                    if (c->isVisible() == false) {
                        return false;
                    }
                }

                m_pSelectedItem = this->itemForTouch(this, touch);
                if (m_pSelectedItem) {
                    m_eState = cocos2d::kCCMenuStateTrackingTouch;
                    m_pSelectedItem->selected();
                    return true;
                }
                return false;
            }

            void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
                auto currentItem = this->itemForTouch(this, touch);
                if (currentItem != m_pSelectedItem) {
                    if (m_pSelectedItem) {
                        m_pSelectedItem->unselected();
                    }
                    m_pSelectedItem = currentItem;
                    if (m_pSelectedItem) {
                        m_pSelectedItem->selected();
                    }
                }
            }

            cocos2d::CCMenuItem* itemForTouch(cocos2d::CCNode* parent, cocos2d::CCTouch* touch) {
                auto touchLocation = touch->getLocation();

                for (auto node : geode::cocos::CCArrayExt<cocos2d::CCNode*>(parent->getChildren())) {
                    if (node->isVisible()) {
                        if (auto item = geode::cast::typeinfo_cast<cocos2d::CCMenuItem*>(node); item && item->isEnabled()) {
                            auto local = item->convertToNodeSpace(touchLocation);
                            auto r = item->rect();
                            r.origin = ccp(0, 0);

                            if (r.containsPoint(local)) {
                                return item;
                            }
                        }

                        if (!geode::cast::typeinfo_cast<cocos2d::CCMenu*>(node)) {
                            if (auto item = this->itemForTouch(node, touch)) {
                                return item;
                            }
                        }
                    }
                }
            
                return NULL;
            }
        };
    }

    struct Menu : public BaseInitializer<Menu> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
    
        cocos2d::CCNode* construct() {
            auto node = impl::MenuWrapper::create();

            (void)utils::applyChild(this, node);
            utils::applySingleConstrainedLayout(this, node);

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}