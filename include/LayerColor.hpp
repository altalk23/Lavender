#pragma once

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class LayerColorWrapper : public cocos2d::CCLayerColor {
        public:
            std::function<void()> keyBackClickedFunction;
            std::function<bool(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchBeganFunction;
            std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchMovedFunction;
            std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchEndedFunction;
            std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchCancelledFunction;
            std::function<void(cocos2d::enumKeyCodes)> keyDownFunction;
            std::function<void(cocos2d::enumKeyCodes)> keyUpFunction;

            static LayerColorWrapper* create(const cocos2d::ccColor4B& color, float width, float height) {
                auto layer = new (std::nothrow) LayerColorWrapper();
                if (layer && layer->initWithColor(color, width, height)) {
                    layer->autorelease();
                    return layer;
                }
                delete layer;
                return nullptr;
            }

            bool initWithColor(const cocos2d::ccColor4B& color, float width, float height) override {
                if (!cocos2d::CCLayerColor::initWithColor(color, width, height)) {
                    return false;
                }
                
                this->setAnchorPoint(ccp(0, 0));
                this->setPosition(ccp(0, 0));

                return true;
            }

            void keyBackClicked() override {
                if (this->keyBackClickedFunction) {
                    this->keyBackClickedFunction();
                }
            }

            bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchBeganFunction) {
                    return this->onTouchBeganFunction(touch, event);
                }
                return false;
            }

            void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchMovedFunction) {
                    this->onTouchMovedFunction(touch, event);
                }
            }

            void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchEndedFunction) {
                    this->onTouchEndedFunction(touch, event);
                }
            }

            void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchCancelledFunction) {
                    this->onTouchCancelledFunction(touch, event);
                }
            }

            void keyDown(cocos2d::enumKeyCodes keyCode) override {
                if (this->keyDownFunction) {
                    this->keyDownFunction(keyCode);
                }
            }

            void keyUp(cocos2d::enumKeyCodes keyCode) override {
                if (this->keyUpFunction) {
                    this->keyUpFunction(keyCode);
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

        std::function<bool(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchBegan;
        std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchMoved;
        std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchEnded;
        std::function<void(cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchCancelled;

        std::function<void(cocos2d::enumKeyCodes)> keyDown;
        std::function<void(cocos2d::enumKeyCodes)> keyUp;

        cocos2d::CCNode* construct() const {
            auto node = impl::LayerColorWrapper::create(this->color, 0.f, 0.f);

            (void)utils::applyChild(this, node);
            utils::applySizedConstrainedLayout(this, node);

            utils::applyID(this, node);
            utils::applySize(this, node);

            if (this->keyBackClicked) {
                node->keyBackClickedFunction = this->keyBackClicked;
                node->setKeypadEnabled(true);
            }

            if (this->onTouchBegan) {
                node->onTouchBeganFunction = this->onTouchBegan;
                node->setTouchEnabled(true);
            }

            if (this->onTouchMoved) {
                node->onTouchMovedFunction = this->onTouchMoved;
            }

            if (this->onTouchEnded) {
                node->onTouchEndedFunction = this->onTouchEnded;
            }

            if (this->onTouchCancelled) {
                node->onTouchCancelledFunction = this->onTouchCancelled;
            }

            if (this->keyDown) {
                node->keyDownFunction = this->keyDown;
                node->setKeyboardEnabled(true);
            }

            if (this->keyUp) {
                node->keyUpFunction = this->keyUp;
                node->setKeyboardEnabled(true);
            }

            delete this;
            return node;
        }
    };
}