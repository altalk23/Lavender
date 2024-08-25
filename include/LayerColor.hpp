#pragma once

#include "Base.hpp"

namespace ui {
    namespace impl {
        class LayerColorWrapper : public cocos2d::CCLayerColor {
        public:
            std::function<void()> keyBackClickedFunction;

            static LayerColorWrapper* create(const cocos2d::ccColor4B& color, float width, float height) {
                auto layer = new (std::nothrow) LayerColorWrapper();
                if (layer && layer->initWithColor(color, width, height)) {
                    layer->autorelease();
                    return layer;
                }
                delete layer;
                return nullptr;
            }

            bool initWithColor(const cocos2d::ccColor4B& color, float width, float height) {
                if (!cocos2d::CCLayerColor::initWithColor(color, width, height)) {
                    return false;
                }
                
                this->setAnchorPoint(ccp(0, 0));
                this->setPosition(ccp(0, 0));

                this->setKeypadEnabled(true);

                return true;
            }

            void keyBackClicked() {
                if (this->keyBackClickedFunction) {
                    this->keyBackClickedFunction();
                }
            }
        };
    }

    struct LayerColor : public BaseInitializer<LayerColor> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
        LAVENDER_ADD_SIZE();
    
        cocos2d::ccColor4B color;

        std::function<void()> keyBackClicked;

        cocos2d::CCNode* construct() {
            auto node = impl::LayerColorWrapper::create(this->color, 0.f, 0.f);

            if (auto child = utils::applyChild(this, node)) {
                utils::applyCopySize(this, node, child);
            }

            utils::applyID(this, node);
            utils::applySize(this, node);

            if (this->keyBackClicked) {
                node->keyBackClickedFunction = this->keyBackClicked;
            }

            delete this;
            return node;
        }
    };
}