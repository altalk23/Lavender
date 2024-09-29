#pragma once

#include "Base.hpp"
#include "Utils.hpp"
#include "ConstrainedLayout.hpp"

namespace ui {
    namespace impl {
        class LayerColorWrapper : public cocos2d::CCLayerColor {
        public:
            std::function<void(cocos2d::CCLayerColor*)> keyBackClickedFunction;
            std::function<bool(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchBeganFunction;
            std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchMovedFunction;
            std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchEndedFunction;
            std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchCancelledFunction;
            std::function<void(cocos2d::CCLayerColor*, cocos2d::enumKeyCodes)> keyDownFunction;
            std::function<void(cocos2d::CCLayerColor*, cocos2d::enumKeyCodes)> keyUpFunction;
            std::function<void(cocos2d::CCLayerColor*)> onEnterFunction;
            std::function<void(cocos2d::CCLayerColor*)> onExitFunction;
            std::function<void(cocos2d::CCLayerColor*)> registerWithTouchDispatcherFunction;
            std::function<void(cocos2d::CCLayerColor*)> initFunction;
            std::function<void(cocos2d::CCLayerColor*)> destructFunction;

            static LayerColorWrapper* create(const cocos2d::ccColor4B& color, float width, float height, std::function<void(cocos2d::CCLayerColor*)> initFunction = nullptr, std::function<void(cocos2d::CCLayerColor*)> destructFunction = nullptr) {
                auto layer = new (std::nothrow) LayerColorWrapper();
                if (layer && layer->initWithColor(color, width, height, initFunction, destructFunction)) {
                    layer->autorelease();
                    return layer;
                }
                delete layer;
                return nullptr;
            }

            bool initWithColor(const cocos2d::ccColor4B& color, float width, float height, std::function<void(cocos2d::CCLayerColor*)> initFunction, std::function<void(cocos2d::CCLayerColor*)> destructFunction) {
                if (!cocos2d::CCLayerColor::initWithColor(color, width, height)) {
                    return false;
                }
                
                this->setAnchorPoint(ccp(0, 0));
                this->setPosition(ccp(0, 0));

                this->initFunction = initFunction;
                this->destructFunction = destructFunction;

                if (this->initFunction) {
                    this->initFunction(this);
                }

                return true;
            }

            ~LayerColorWrapper() {
                if (this->destructFunction) {
                    this->destructFunction(this);
                }
            }

            void onEnter() override {
                if (this->onEnterFunction) {
                    this->onEnterFunction(this);
                }
                else {
                    cocos2d::CCLayerColor::onEnter();
                }
            }

            void onExit() override {
                if (this->onExitFunction) {
                    this->onExitFunction(this);
                }
                else {
                    cocos2d::CCLayerColor::onExit();
                }
            }

            void registerWithTouchDispatcher() override {
                if (this->registerWithTouchDispatcherFunction) {
                    this->registerWithTouchDispatcherFunction(this);
                }
                else {
                    cocos2d::CCLayerColor::registerWithTouchDispatcher();
                }
            }

            void keyBackClicked() override {
                if (this->keyBackClickedFunction) {
                    this->keyBackClickedFunction(this);
                }
                else {
                    cocos2d::CCLayerColor::keyBackClicked();
                }
            }

            bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchBeganFunction) {
                    return this->onTouchBeganFunction(this, touch, event);
                }
                else {
                    return cocos2d::CCLayerColor::ccTouchBegan(touch, event);
                }
            }

            void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchMovedFunction) {
                    this->onTouchMovedFunction(this, touch, event);
                }
                else {
                    cocos2d::CCLayerColor::ccTouchMoved(touch, event);
                }
            }

            void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchEndedFunction) {
                    this->onTouchEndedFunction(this, touch, event);
                }
                else {
                    cocos2d::CCLayerColor::ccTouchEnded(touch, event);
                }
            }

            void ccTouchCancelled(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override {
                if (this->onTouchCancelledFunction) {
                    this->onTouchCancelledFunction(this, touch, event);
                }
                else {
                    cocos2d::CCLayerColor::ccTouchCancelled(touch, event);
                }
            }

            void keyDown(cocos2d::enumKeyCodes keyCode) override {
                if (this->keyDownFunction) {
                    this->keyDownFunction(this, keyCode);
                }
                else {
                    cocos2d::CCLayerColor::keyDown(keyCode);
                }
            }

            void keyUp(cocos2d::enumKeyCodes keyCode) override {
                if (this->keyUpFunction) {
                    this->keyUpFunction(this, keyCode);
                }
                else {
                    cocos2d::CCLayerColor::keyUp(keyCode);
                }
            }
        };
    }

    struct LayerColor : public BaseInitializer<LayerColor> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_SIZE();
        Color4B color = cocos2d::ccc4(0, 0, 0, 255);

        std::function<void(cocos2d::CCLayerColor*)> keyBackClicked;

        std::function<bool(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchBegan;
        std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchMoved;
        std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchEnded;
        std::function<void(cocos2d::CCLayerColor*, cocos2d::CCTouch*, cocos2d::CCEvent*)> onTouchCancelled;

        std::function<void(cocos2d::CCLayerColor*, cocos2d::enumKeyCodes)> keyDown;
        std::function<void(cocos2d::CCLayerColor*, cocos2d::enumKeyCodes)> keyUp;

        std::function<void(cocos2d::CCLayerColor*)> onEnter;
        std::function<void(cocos2d::CCLayerColor*)> onExit;
        std::function<void(cocos2d::CCLayerColor*)> registerWithTouchDispatcher;
        std::function<void(cocos2d::CCLayerColor*)> init;
        std::function<void(cocos2d::CCLayerColor*)> destruct;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
            auto node = impl::LayerColorWrapper::create(this->color, 0.f, 0.f, this->init, this->destruct);

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

            if (this->onEnter) {
                node->onEnterFunction = this->onEnter;
            }

            if (this->onExit) {
                node->onExitFunction = this->onExit;
            }

            if (this->registerWithTouchDispatcher) {
                node->registerWithTouchDispatcherFunction = this->registerWithTouchDispatcher;
            }

            delete this;
            return node;
        }
    };
}