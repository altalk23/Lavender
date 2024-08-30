#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class TextInputLayout : public cocos2d::Layout {
        public:
            static TextInputLayout* create() {
                auto ret = new (std::nothrow) TextInputLayout();
                if (ret) {
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                auto node = geode::cast::typeinfo_cast<geode::TextInput*>(in);
                if (!node) return;

                node->setWidth(maxSize.width);

                if (minSize.height > node->getContentHeight()) {
                    node->setContentHeight(minSize.height);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct TextInput : BaseInitializer<TextInput> {
        LAVENDER_ADD_ID();

        std::string placeholder;
        std::string font = "bigFont.fnt";
        std::optional<std::string> label;
        std::optional<std::string> text;

        TextAlignment alignment = TextAlignment::Center;
        std::variant<geode::CommonFilter, std::string> filter = geode::CommonFilter::Any;

        std::optional<size_t> maxCharCount;
        bool password = false;
        TextInputDelegate* delegate = nullptr;

        std::function<void(std::string const&)> callback;

        bool enabled = true;
        bool hideBackground = false;

        std::optional<cocos2d::ccColor3B> textColor;
        std::optional<cocos2d::ccColor3B> placeholderColor;
        std::optional<cocos2d::ccColor4B> backgroundColor;

        bool autoFocus = false;

        cocos2d::CCNode* construct() const {
            auto node = geode::TextInput::create(100, this->placeholder, this->font);

            utils::applyID(this, node);
            node->setLayout(impl::TextInputLayout::create());

            if (this->label.has_value()) {
                node->setLabel(this->label.value());
            }

            if (this->text.has_value()) {
                node->setString(this->text.value());
            }

            switch (this->alignment) {
                case TextAlignment::Center:
                    node->setTextAlign(geode::TextInputAlign::Center);
                    break;
                case TextAlignment::Left:
                    node->setTextAlign(geode::TextInputAlign::Left);
                    break;
                default:
                    break;
            }

            if (std::holds_alternative<geode::CommonFilter>(this->filter)) {
                node->setCommonFilter(std::get<geode::CommonFilter>(this->filter));
            }
            else {
                node->setFilter(std::get<std::string>(this->filter));
            }

            if (this->maxCharCount.has_value()) {
                node->setMaxCharCount(this->maxCharCount.value());
            }

            node->setPasswordMode(this->password);

            if (this->delegate) {
                node->setDelegate(this->delegate);
            }

            if (this->callback) {
                node->setCallback(this->callback);
            }

            node->setEnabled(this->enabled);

            if (this->hideBackground) {
                node->hideBG();
            }

            if (this->textColor.has_value()) {
                node->getInputNode()->m_textColor = this->textColor.value();
                node->getInputNode()->refreshLabel();
            }

            if (this->placeholderColor.has_value()) {
                node->getInputNode()->m_placeholderColor = this->placeholderColor.value();
                node->getInputNode()->refreshLabel();
            }

            if (this->backgroundColor.has_value()) {
                auto color = this->backgroundColor.value();
                node->getBGSprite()->setColor({ color.r, color.g, color.b });
                node->getBGSprite()->setOpacity(color.a);
            }

            // if (this->autoFocus) {
            //     node->focus();
            // }

            return node;
        }
    };
}