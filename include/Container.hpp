#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct EdgeInsets {
        float top = 0.f;
        float right = 0.f;
        float bottom = 0.f;
        float left = 0.f;

        struct All {
            float all = 0.f;

            operator EdgeInsets() const {
                return { all, all, all, all };
            }
        };

        struct Symmetric {
            float vertical = 0.f;
            float horizontal = 0.f;

            operator EdgeInsets() const {
                return { vertical, horizontal, vertical, horizontal };
            }
        };
    };

    namespace impl {
        class ContainerLayout : public cocos2d::Layout {
        public:
            std::optional<float> m_width;
            std::optional<float> m_height;
            EdgeInsets m_padding;

            static ContainerLayout* create(EdgeInsets padding, std::optional<float> width, std::optional<float> height) {
                auto ret = new (std::nothrow) ContainerLayout();
                if (ret) {
                    ret->m_padding = padding;
                    ret->m_width = width;
                    ret->m_height = height;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }
            virtual ~ContainerLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                if (m_width.has_value()) {
                    minSize.width = std::clamp(m_width.value(), minSize.width, maxSize.width);
                    maxSize.width = std::clamp(m_width.value(), minSize.width, maxSize.width);
                }
                if (m_height.has_value()) {
                    minSize.height = std::clamp(m_height.value(), minSize.height, maxSize.height);
                    maxSize.height = std::clamp(m_height.value(), minSize.height, maxSize.height);
                }

                if (auto child = utils::getChild(in)) {
                    auto const paddingSize = cocos2d::CCSize(
                        m_padding.left + m_padding.right,
                        m_padding.top + m_padding.bottom
                    );

                    minSize.width = std::max(minSize.width - paddingSize.width, 0.f);
                    minSize.height = std::max(minSize.height - paddingSize.height, 0.f);
                    maxSize.width = std::max(maxSize.width - paddingSize.width, 0.f);
                    maxSize.height = std::max(maxSize.height - paddingSize.height, 0.f);


                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();
                    
                    auto const size = child->getContentSize();
                    auto const center = size / 2.f + cocos2d::CCSize(
                        m_padding.left,
                        m_padding.bottom
                    );
                    in->setContentSize(size + paddingSize);

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(center);
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

    struct Container : BaseInitializer<Container> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILD();
        LAVENDER_ADD_SIZE();

        EdgeInsets padding;

        bool hidden = false;

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            if (auto child = utils::applyChild(this, node)) {
                if (this->size.has_value()) {
                    node->setLayout(
                        impl::ContainerLayout::create(this->padding, this->size->width, this->size->height)
                    );
                }
                else {
                    node->setLayout(
                        impl::ContainerLayout::create(this->padding, this->width, this->height)
                    );
                }
            }

            utils::applyID(this, node);
            utils::applySize(this, node);

            node->setVisible(!this->hidden);

            delete this;
            return node;
        }
    };
}