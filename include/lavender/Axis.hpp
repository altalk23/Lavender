#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "Expanded.hpp"

namespace ui {
    namespace impl {
        class ColumnLayout : public cocos2d::Layout {
        public:
            MainAxisAlignment m_mainAxis;
            CrossAxisAlignment m_crossAxis;
            VerticalDirection m_direction;

            static ColumnLayout* create(
                MainAxisAlignment mainAxis, 
                CrossAxisAlignment crossAxis,
                VerticalDirection direction
            ) {
                auto ret = new (std::nothrow) ColumnLayout();
                if (ret) {
                    ret->m_mainAxis = mainAxis;
                    ret->m_crossAxis = crossAxis;
                    ret->m_direction = direction;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            virtual std::pair<cocos2d::CCSize, cocos2d::CCSize> getConstraints(cocos2d::CCNode* in) const {
                return utils::getConstraints(in);
            }

            virtual void setConstraints(cocos2d::CCNode* node, cocos2d::CCSize minSize, cocos2d::CCSize maxSize) const {
                utils::setConstraints(node, minSize, maxSize);
            }

            virtual Axis getMainAxis() const {
                return Axis::Vertical;
            }

            void apply(cocos2d::CCNode* in) override {
                auto const [minSize, maxSize] = this->getConstraints(in);
                
                float totalHeight = 0.0f;
                float totalWidth = 0.0f;

                size_t flexCount = 0;

                std::vector<Expanded*> expandeds;

                for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {
                    auto expanded = geode::cast::typeinfo_cast<Expanded*>(child);
                    if (expanded && expanded->getABIVersion() >= 1) {
                        flexCount += expanded->getFlex();
                        expandeds.push_back(expanded);
                    }
                    else {
                        if (m_crossAxis == CrossAxisAlignment::Stretch) {
                            this->setConstraints(child, ccp(maxSize.width, 0), ccp(maxSize.width, FLT_MAX));
                        }
                        else {
                            this->setConstraints(child, ccp(0, 0), ccp(maxSize.width, FLT_MAX));
                        }
                        child->updateLayout();
                        totalHeight += this->getNodeHeight(child);
                        totalWidth = std::max(totalWidth, this->getNodeWidth(child));
                    }
                }

                if (totalHeight > maxSize.height) {
                    this->warnConstraintViolation(totalHeight, maxSize.height);
                }

                float const remainingHeight = maxSize.height - totalHeight;

                for (auto expanded : expandeds) {
                    if (expanded->getABIVersion() >= 1) {
                        auto const height = remainingHeight / flexCount * expanded->getFlex();

                        expanded->setMainAxis(this->getMainAxis());
                        this->setConstraints(expanded, ccp(0, 0), ccp(maxSize.width, height));
                    }
                    expanded->updateLayout();

                    totalWidth = std::max(totalWidth, this->getNodeWidth(expanded));
                }

                totalWidth = std::max(totalWidth, minSize.width);

                this->setContentWidth(in, totalWidth);
                if (maxSize.height == FLT_MAX) {
                    this->setContentHeight(in, std::max(totalHeight, minSize.height));
                }
                else {
                    this->setContentHeight(in, maxSize.height);
                }

                float gap = remainingHeight;
                float offset = 0.0f;
                float flexGap = 0.0f;

                if (flexCount > 0) {
                    flexGap = gap / flexCount;
                    gap = 0.f;
                    offset = 0.0f;
                }
                else {
                    switch (m_mainAxis) {
                        case MainAxisAlignment::Start:
                            offset = 0.0f;
                            gap = 0.0f;
                            break;
                        case MainAxisAlignment::Center:
                            offset = gap / 2.0f;
                            gap = 0.0f;
                            break;
                        case MainAxisAlignment::End:
                            offset = gap;
                            gap = 0.0f;
                            break;
                        case MainAxisAlignment::Even:
                            gap = gap / (in->getChildrenCount() + 1);
                            offset = gap;
                            break;
                        case MainAxisAlignment::Between:
                            gap = gap / (in->getChildrenCount() - 1);
                            offset = 0.0f;
                            break;
                        case MainAxisAlignment::Around:
                            gap = gap / in->getChildrenCount();
                            offset = gap / 2.0f;
                            break;
                    }
                }

                if (m_direction == VerticalDirection::TopToBottom) {
                    offset = this->getNodeHeight(in) - offset;
                }

                for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {                    
                    auto center = this->getNodeSize(child) / 2.0f;
                    if (m_direction == VerticalDirection::TopToBottom) {
                        center.height = -center.height;
                    }
                    auto const endWidth = this->getNodeWidth(in) - this->getNodeWidth(child);
                    switch (m_crossAxis) {
                        case CrossAxisAlignment::Start:
                            this->setNodePosition(child, ccp(0, offset) + center);
                            break;
                        case CrossAxisAlignment::Stretch: [[fallthrough]];
                        case CrossAxisAlignment::Center:
                            this->setNodePosition(child, ccp(endWidth / 2.0f, offset) + center);
                            break;
                        case CrossAxisAlignment::End:
                            this->setNodePosition(child, ccp(endWidth, offset) + center);
                            break;
                    }
                    child->ignoreAnchorPointForPosition(false);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));

                    switch (m_direction) {
                        case VerticalDirection::TopToBottom:
                            offset -= this->getNodeHeight(child) + gap;
                            break;
                        case VerticalDirection::BottomToTop:
                            offset += this->getNodeHeight(child) + gap;
                            break;
                    }
                }
            }

            virtual void warnConstraintViolation(float height, float maxHeight) {
                geode::log::warn("Column height constraint violation {} > {}", height, maxHeight);
            }

            virtual cocos2d::CCSize getNodeSize(cocos2d::CCNode* node) const {
                return node->getContentSize();
            }

            virtual void setContentWidth(cocos2d::CCNode* node, float width) const {
                node->setContentWidth(width);
            }

            virtual void setContentHeight(cocos2d::CCNode* node, float height) const {
                node->setContentHeight(height);
            }

            virtual void setNodePosition(cocos2d::CCNode* node, cocos2d::CCPoint position) const {
                node->setPosition(position);
            }

            virtual float getNodeWidth(cocos2d::CCNode* node) const {
                return node->getContentSize().width;
            }

            virtual float getNodeHeight(cocos2d::CCNode* node) const {
                return node->getContentSize().height;
            }

            cocos2d::CCSize getSizeHint(cocos2d::CCNode* in) const override {
                return in->getContentSize();
            }
        };

        class RowLayout : public ColumnLayout {
        public:
            static ColumnLayout* create(
                MainAxisAlignment mainAxis, 
                CrossAxisAlignment crossAxis,
                HorizontalDirection direction
            ) {
                auto ret = new (std::nothrow) RowLayout();
                if (ret) {
                    ret->m_mainAxis = mainAxis;
                    ret->m_crossAxis = crossAxis;
                    ret->m_direction = direction == HorizontalDirection::LeftToRight ? VerticalDirection::BottomToTop : VerticalDirection::TopToBottom;
                    ret->autorelease();
                    return ret;
                }
                return nullptr;
            }

            void warnConstraintViolation(float width, float maxWidth) override {
                geode::log::warn("Row width constraint violation {} > {}", width, maxWidth);
            }

            std::pair<cocos2d::CCSize, cocos2d::CCSize> getConstraints(cocos2d::CCNode* in) const override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                return std::pair<cocos2d::CCSize, cocos2d::CCSize>(
                    cocos2d::CCSize(minSize.height, minSize.width),
                    cocos2d::CCSize(maxSize.height, maxSize.width)
                );
            }

            void setConstraints(cocos2d::CCNode* node, cocos2d::CCSize minSize, cocos2d::CCSize maxSize) const override {
                utils::setConstraints(node, ccp(minSize.height, minSize.width), ccp(maxSize.height, maxSize.width));
            }

            Axis getMainAxis() const override {
                return Axis::Horizontal;
            }

            cocos2d::CCSize getNodeSize(cocos2d::CCNode* node) const override {
                return cocos2d::CCSize(node->getContentSize().height, node->getContentSize().width);
            }

            void setContentWidth(cocos2d::CCNode* node, float width) const override {
                node->setContentHeight(width);
            }

            void setContentHeight(cocos2d::CCNode* node, float height) const override {
                node->setContentWidth(height);
            }

            void setNodePosition(cocos2d::CCNode* node, cocos2d::CCPoint position) const override {
                node->setPosition(ccp(position.y, position.x));
            }

            float getNodeWidth(cocos2d::CCNode* node) const override {
                return node->getContentSize().height;
            }

            float getNodeHeight(cocos2d::CCNode* node) const override {
                return node->getContentSize().width;
            }
        };
    }

    struct Column : public BaseInitializer<Column> {
        LAVENDER_ADD_ID();

        MainAxisAlignment mainAxis = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxis = CrossAxisAlignment::Center;
        VerticalDirection direction = VerticalDirection::TopToBottom;

        LAVENDER_ADD_CHILDREN();
        LAVENDER_ADD_CHILDREN_BUILDER();

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            utils::applyID(this, node);

            if (!utils::applyChildrenBuilder(this, node)) {
                (void)utils::applyChildren(this, node);
            }

            node->setLayout(
                impl::ColumnLayout::create(mainAxis, crossAxis, direction)
            );

            delete this;
            return node;
        }
    };

    struct Row : public BaseInitializer<Row> {
        LAVENDER_ADD_ID();

        MainAxisAlignment mainAxis = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxis = CrossAxisAlignment::Center;
        HorizontalDirection direction = HorizontalDirection::LeftToRight;

        LAVENDER_ADD_CHILDREN();
        LAVENDER_ADD_CHILDREN_BUILDER();

        cocos2d::CCNode* construct() const {
            auto node = cocos2d::CCNode::create();

            utils::applyID(this, node);

            if (!utils::applyChildrenBuilder(this, node)) {
                (void)utils::applyChildren(this, node);
            }
            
            node->setLayout(
                impl::RowLayout::create(mainAxis, crossAxis, direction)
            );

            delete this;
            return node;
        }
    };
}