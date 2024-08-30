#pragma once

#include <Geode/Geode.hpp>

#include "Utils.hpp"

namespace ui {
    namespace impl {
        class SingleConstrainedLayout : public cocos2d::Layout {
        public:
            static SingleConstrainedLayout* create() {
                auto ret = new (std::nothrow) SingleConstrainedLayout();
                if (ret) {
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

        class SizedConstrainedLayout : public cocos2d::Layout {
        public:
            std::optional<float> m_width;
            std::optional<float> m_height;

            static SizedConstrainedLayout* create(std::optional<float> width, std::optional<float> height) {
                auto ret = new (std::nothrow) SizedConstrainedLayout();
                if (ret) {
                    ret->m_width = width;
                    ret->m_height = height;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

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
                    utils::setConstraints(child, minSize, maxSize);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize());

                    child->ignoreAnchorPointForPosition(false);
                    child->setPosition(in->getContentSize() / 2.f);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                }
                else {
                    in->setContentSize(maxSize);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };
    }

    namespace utils {
        void applySingleConstrainedLayout(auto const* data, cocos2d::CCNode* node) {
            node->setLayout(
                impl::SingleConstrainedLayout::create()
            );
        }

        void applySizedConstrainedLayout(auto const* data, cocos2d::CCNode* node) {
            if (data->size.has_value()) {
                node->setLayout(
                    impl::SizedConstrainedLayout::create(data->size->width, data->size->height)
                );
            }
            else {
                node->setLayout(
                    impl::SizedConstrainedLayout::create(data->width, data->height)
                );
            }
        }
    }
}