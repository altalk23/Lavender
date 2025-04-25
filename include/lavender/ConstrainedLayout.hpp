#pragma once

#include <Geode/Geode.hpp>

#include "Utils.hpp"

namespace ui {
    namespace impl {
        class SingleConstrainedLayout : public geode::Layout {
        public:
            bool m_updatePosition;

            static SingleConstrainedLayout* create(bool updatePosition = true) {
                auto ret = new (std::nothrow) SingleConstrainedLayout();
                if (ret) {
                    ret->m_updatePosition = updatePosition;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                if (in->getChildrenCount() > 0) {
                    for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {
                        utils::setConstraints(child, minSize, maxSize);
                        child->updateLayout();

                        in->setContentSize(child->getContentSize());

                        if (m_updatePosition) {
                            child->ignoreAnchorPointForPosition(false);
                            child->setPosition(in->getContentSize() / 2.f);
                            child->setAnchorPoint(ccp(0.5f, 0.5f));
                        }
                    }
                }
                else {
                    in->setContentSize(minSize);
                }
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };

        class ScaledConstrainedLayout : public geode::Layout {
        public:
            static ScaledConstrainedLayout* create() {
                auto ret = new (std::nothrow) ScaledConstrainedLayout();
                if (ret) {
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = utils::getConstraints(in);
                if (auto child = utils::getChild(in)) {
                    auto scale = ccp(std::fabs(child->getScaleX()), std::fabs(child->getScaleY()));
                    utils::setConstraints(child, minSize * scale, maxSize * scale);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize() * scale);

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

        class SizedConstrainedLayout : public geode::Layout {
        public:
            std::optional<float> m_width;
            std::optional<float> m_height;
            bool m_hasChild;

            static SizedConstrainedLayout* create(std::optional<float> width, std::optional<float> height, bool hasChild = true) {
                auto ret = new (std::nothrow) SizedConstrainedLayout();
                if (ret) {
                    ret->m_width = width;
                    ret->m_height = height;
                    ret->m_hasChild = hasChild;
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

                if (auto child = utils::getChild(in); m_hasChild && child) {
                    auto scale = ccp(std::fabs(child->getScaleX()), std::fabs(child->getScaleY()));
                    utils::setConstraints(child, minSize * scale, maxSize * scale);
                    child->updateLayout();

                    in->setContentSize(child->getContentSize() * scale);

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

        class PrimarySizedConstrainedLayout : public geode::Layout {
        public:
            std::optional<float> m_width;
            std::optional<float> m_height;
            cocos2d::CCNode* m_primaryChild;

            static PrimarySizedConstrainedLayout* create(std::optional<float> width, std::optional<float> height, cocos2d::CCNode* primaryChild) {
                auto ret = new (std::nothrow) PrimarySizedConstrainedLayout();
                if (ret) {
                    ret->m_width = width;
                    ret->m_height = height;
                    ret->m_primaryChild = primaryChild; 
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

                if (m_primaryChild) {
                    utils::setConstraints(m_primaryChild, minSize, maxSize);
                    m_primaryChild->updateLayout();

                    auto primarySize = m_primaryChild->getContentSize();
                    in->setContentSize(primarySize);

                    m_primaryChild->ignoreAnchorPointForPosition(false);
                    m_primaryChild->setPosition(in->getContentSize() / 2.f);
                    m_primaryChild->setAnchorPoint(ccp(0.5f, 0.5f));

                    for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {
                        if (child != m_primaryChild) {
                            auto scale = ccp(std::fabs(child->getScaleX()), std::fabs(child->getScaleY()));
                            auto inverseScale = ccp(1.f / scale.x, 1.f / scale.y);
                            utils::setConstraints(child, primarySize * inverseScale, primarySize * inverseScale);
                            child->updateLayout();

                            child->ignoreAnchorPointForPosition(false);
                            child->setPosition(in->getContentSize() / 2.f);
                            child->setAnchorPoint(ccp(0.5f, 0.5f));
                        }
                    }
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
        void applySingleConstrainedLayout(auto const* data, cocos2d::CCNode* node, bool updatePosition = true) {
            node->setLayout(
                impl::SingleConstrainedLayout::create(updatePosition)
            );
        }

        void applyScaledConstrainedLayout(auto const* data, cocos2d::CCNode* node) {
            node->setLayout(
                impl::ScaledConstrainedLayout::create()
            );
        }

        void applySizedConstrainedLayout(auto const* data, cocos2d::CCNode* node, bool hasChild = true) {
            if (data->size.has_value()) {
                node->setLayout(
                    impl::SizedConstrainedLayout::create(data->size->width, data->size->height, hasChild)
                );
            }
            else {
                node->setLayout(
                    impl::SizedConstrainedLayout::create(data->width, data->height, hasChild)
                );
            }
        }

        void applyPrimarySizedConstrainedLayout(auto const* data, cocos2d::CCNode* node, cocos2d::CCNode* primaryChild) {
            if (data->size.has_value()) {
                node->setLayout(
                    impl::PrimarySizedConstrainedLayout::create(data->size->width, data->size->height, primaryChild)
                );
            }
            else {
                node->setLayout(
                    impl::PrimarySizedConstrainedLayout::create(data->width, data->height, primaryChild)
                );
            }
        }
    }
}