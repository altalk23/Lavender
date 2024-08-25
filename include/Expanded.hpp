#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class Expanded : public cocos2d::CCNode {
        public:
            size_t m_flex;

            static Expanded* create(size_t flex) {
                auto ret = new Expanded();
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

            if (auto child = utils::applyChild(this, node)) {
                utils::applyCopySize(this, node, child);
            }

            utils::applyID(this, node);

            delete this;
            return node;
        }
    };
}