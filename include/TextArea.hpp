#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    enum class TextWrapping {
        None,
        Word,
        Space,
        Cutoff,
    };

    namespace impl {
        class TextAreaLayout : public cocos2d::Layout {
        public:
            geode::Ref<cocos2d::CCLayerColor> m_constraintError = nullptr;
            bool m_constraintErrorAdded = false;

            static TextAreaLayout* create() {
                auto ret = new (std::nothrow) TextAreaLayout();
                if (ret) {
                    ret->autorelease();
                    ret->m_constraintError = cocos2d::CCLayerColor::create(cocos2d::ccc4(255, 0, 0, 128));
                    ret->m_constraintError->setVisible(false);
                    ret->m_constraintError->setAnchorPoint(cocos2d::CCPoint(0.5f, 0.5f));

                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                auto node = geode::cast::typeinfo_cast<geode::SimpleTextArea*>(in);
                if (!node) return;
                
                node->setWidth(maxSize.width);
                node->setWidth(maxSize.width);

                auto container = utils::getChild(in);
                auto rect = geode::cocos::calculateChildCoverage(container);

                auto width = std::max(rect.size.width, minSize.width);

                node->setWidth(width);
                node->setWidth(width);

                auto height = node->getHeight();

                if (height > maxSize.height) {
                    // protection against constraint violation
                    // TODO: add visual indication of constraint violation
                    geode::log::warn("TextArea height constraint violation {} > {}", height, maxSize.height);
                    node->setContentHeight(maxSize.height);
                    height = maxSize.height;

                    if (!m_constraintErrorAdded) {
                        in->addChild(m_constraintError);
                        m_constraintErrorAdded = true;
                    }

                    m_constraintError->setContentSize(cocos2d::CCSize(width, height));
                    m_constraintError->setPosition(cocos2d::CCPoint(0, 0));
                    m_constraintError->setVisible(true);
                }
                else {
                    m_constraintError->setVisible(false);
                }

                if (width > maxSize.width) {
                    // protection against constraint violation
                    // TODO: add visual indication of constraint violation
                    geode::log::warn("TextArea width constraint violation {} > {}", rect.size.width, maxSize.width);
                    node->setContentWidth(maxSize.width);
                    width = maxSize.width;

                    if (!m_constraintErrorAdded) {
                        in->addChild(m_constraintError);
                        m_constraintErrorAdded = true;
                    }

                    m_constraintError->setContentSize(cocos2d::CCSize(width, height));
                    m_constraintError->setPosition(cocos2d::CCPoint(0, 0));
                    m_constraintError->setVisible(true);
                }
                else {
                    m_constraintError->setVisible(false);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    struct TextArea : BaseInitializer<TextArea> {
        LAVENDER_ADD_ID();

        std::string text;
        std::string font = "chatFont.fnt";

        cocos2d::ccColor4B color = cocos2d::ccc4(255, 255, 255, 255);
        TextAlignment alignment = TextAlignment::Left;
        TextWrapping wrapping = TextWrapping::Word;

        std::optional<float> scale;
        std::optional<size_t> maxLines;
        std::optional<float> linePadding;

        cocos2d::CCNode* construct() const {
            auto node = geode::SimpleTextArea::create(this->text, this->font);

            utils::applyID(this, node);

            node->setLayout(impl::TextAreaLayout::create());

            node->setColor(this->color);
            switch (this->alignment) {
                case TextAlignment::Left:
                    node->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentLeft);
                    break;
                case TextAlignment::Center:
                    node->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentCenter);
                    break;
                case TextAlignment::Right:
                    node->setAlignment(cocos2d::CCTextAlignment::kCCTextAlignmentRight);
                    break;
            }

            switch (this->wrapping) {
                case TextWrapping::None:
                    node->setWrappingMode(geode::WrappingMode::NO_WRAP);
                    break;
                case TextWrapping::Word:
                    node->setWrappingMode(geode::WrappingMode::WORD_WRAP);
                    break;
                case TextWrapping::Space:
                    node->setWrappingMode(geode::WrappingMode::SPACE_WRAP);
                    break;
                case TextWrapping::Cutoff:
                    node->setWrappingMode(geode::WrappingMode::CUTOFF_WRAP);
                    break;
            }

            if (this->scale.has_value()) {
                node->setScale(this->scale.value());
            }

            if (this->maxLines.has_value()) {
                node->setMaxLines(this->maxLines.value());
            }

            if (this->linePadding.has_value()) {
                node->setLinePadding(this->linePadding.value());
            }

            delete this;
            return node;
        }
    };
}