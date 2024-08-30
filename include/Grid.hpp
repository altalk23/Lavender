#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    namespace impl {
        class ColumnGridLayout : public cocos2d::Layout {
        public:
            VerticalDirection m_direction;
            HorizontalDirection m_crossDirection;
            float m_mainAxisSpacing;
            float m_crossAxisSpacing;
            size_t m_crossAxisCount;
            float m_childAspectRatio;

            static ColumnGridLayout* create(
                VerticalDirection direction,
                HorizontalDirection crossDirection,
                float mainAxisSpacing,
                float crossAxisSpacing,
                size_t crossAxisCount,
                float childAspectRatio
            ) {
                auto ret = new (std::nothrow) ColumnGridLayout();
                if (ret) {
                    ret->m_direction = direction;
                    ret->m_crossDirection = crossDirection;
                    ret->m_mainAxisSpacing = mainAxisSpacing;
                    ret->m_crossAxisSpacing = crossAxisSpacing;
                    ret->m_crossAxisCount = crossAxisCount;
                    ret->m_childAspectRatio = childAspectRatio;
                    ret->autorelease();
                }
                return ret;
            }

            virtual std::pair<cocos2d::CCSize, cocos2d::CCSize> getConstraints(cocos2d::CCNode* in) const {
                return utils::getConstraints(in);
            }

            virtual void setConstraints(cocos2d::CCNode* in, cocos2d::CCSize minSize, cocos2d::CCSize maxSize) const {
                utils::setConstraints(in, minSize, maxSize);
            }

            void apply(cocos2d::CCNode* in) override {
                auto [minSize, maxSize] = this->getConstraints(in);

                auto const columnCount = m_crossAxisCount;
                auto const columnSpaceCount = columnCount - 1;

                auto const rowCount = (in->getChildrenCount() + columnCount - 1) / columnCount;
                auto const rowSpaceCount = rowCount - 1;

                float crossAxisSpace = maxSize.width - columnSpaceCount * m_crossAxisSpacing;
                float singleCrossAxisSpace = crossAxisSpace / m_crossAxisCount;

                float singleMainAxisSpace = singleCrossAxisSpace / m_childAspectRatio;

                auto singleSpace = cocos2d::CCSize(singleCrossAxisSpace, singleMainAxisSpace);

                this->setContentWidth(in, maxSize.width);
                if (maxSize.height == FLT_MAX) {
                    auto const totalHeight = singleMainAxisSpace * rowCount + m_mainAxisSpacing * (rowCount - 1);
                    this->setContentHeight(in, std::max(minSize.height, totalHeight));
                }
                else {
                    this->setContentHeight(in, maxSize.height);
                }

                std::vector<float> mainAxisOffsets(m_crossAxisCount, 0.0f);
                float crossAxisOffset = 0.0f;

                for (size_t i = 0; i < in->getChildrenCount(); i++) {
                    auto child = static_cast<cocos2d::CCNode*>(in->getChildren()->objectAtIndex(i));

                    this->setConstraints(child, singleSpace, singleSpace);
                    child->updateLayout();

                    auto childSize = this->getNodeSize(child);
                    auto crossAxisIndex = i % m_crossAxisCount;

                    auto const center = this->getNodeSize(child) / 2.0f;
                    switch (m_direction) {
                        case VerticalDirection::BottomToTop:
                            this->setNodePositionY(child, mainAxisOffsets[crossAxisIndex] + center.height);
                            break;
                        case VerticalDirection::TopToBottom:
                            this->setNodePositionY(child, this->getNodeHeight(in) - mainAxisOffsets[crossAxisIndex] - center.height);
                            break;
                    }
                    switch (m_crossDirection) {
                        case HorizontalDirection::LeftToRight:
                            this->setNodePositionX(child, crossAxisOffset + center.width);
                            break;
                        case HorizontalDirection::RightToLeft:
                            this->setNodePositionX(child, this->getNodeWidth(in) - crossAxisOffset - center.width);
                            break;
                    }
                    child->ignoreAnchorPointForPosition(false);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));

                    crossAxisOffset += singleCrossAxisSpace + m_crossAxisSpacing;
                    mainAxisOffsets[crossAxisIndex] += childSize.height + m_mainAxisSpacing;

                    if (crossAxisIndex == m_crossAxisCount - 1) {
                        crossAxisOffset = 0.0f;
                    }
                }
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

            virtual void setNodePositionX(cocos2d::CCNode* node, float x) const {
                node->setPositionX(x);
            }

            virtual void setNodePositionY(cocos2d::CCNode* node, float y) const {
                node->setPositionY(y);
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

        class RowGridLayout : public ColumnGridLayout {
        public:
            static RowGridLayout* create(
                HorizontalDirection direction,
                VerticalDirection crossDirection,
                float mainAxisSpacing,
                float crossAxisSpacing,
                size_t crossAxisCount,
                float childAspectRatio
            ) {
                auto ret = new (std::nothrow) RowGridLayout();
                if (ret) {
                    ret->m_direction = direction == HorizontalDirection::LeftToRight ? VerticalDirection::BottomToTop : VerticalDirection::TopToBottom;
                    ret->m_crossDirection = crossDirection == VerticalDirection::TopToBottom ? HorizontalDirection::RightToLeft : HorizontalDirection::LeftToRight;
                    ret->m_mainAxisSpacing = mainAxisSpacing;
                    ret->m_crossAxisSpacing = crossAxisSpacing;
                    ret->m_crossAxisCount = crossAxisCount;
                    ret->m_childAspectRatio = childAspectRatio;
                    ret->autorelease();
                }
                return ret;
            }

            std::pair<cocos2d::CCSize, cocos2d::CCSize> getConstraints(cocos2d::CCNode* in) const override {
                auto [minSize, maxSize] = utils::getConstraints(in);
                return std::pair<cocos2d::CCSize, cocos2d::CCSize>(
                    cocos2d::CCSize(minSize.height, minSize.width),
                    cocos2d::CCSize(maxSize.height, maxSize.width)
                );
            }

            void setConstraints(cocos2d::CCNode* in, cocos2d::CCSize minSize, cocos2d::CCSize maxSize) const override {
                utils::setConstraints(in, cocos2d::CCSize(minSize.height, minSize.width), cocos2d::CCSize(maxSize.height, maxSize.width));
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

            void setNodePositionX(cocos2d::CCNode* node, float x) const override {
                node->setPositionY(x);
            }

            void setNodePositionY(cocos2d::CCNode* node, float y) const override {
                node->setPositionX(y);
            }

            float getNodeWidth(cocos2d::CCNode* node) const override {
                return node->getContentSize().height;
            }

            float getNodeHeight(cocos2d::CCNode* node) const override {
                return node->getContentSize().width;
            }
        };
    }

    struct Grid : public BaseInitializer<Grid> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_CHILDREN();
        LAVENDER_ADD_CHILDREN_BUILDER();
        
        float mainAxisSpacing = 0.f;
        float crossAxisSpacing = 0.f;
        VerticalDirection verticalDirection = VerticalDirection::TopToBottom;
        HorizontalDirection horizontalDirection = HorizontalDirection::LeftToRight;
        Axis axis = Axis::Vertical;
        size_t crossAxisCount = 2;
        float childAspectRatio = 1.f;

        cocos2d::CCNode* construct() const {
            if (crossAxisCount < 1) {
                delete this;
                return nullptr;
            }

            auto node = cocos2d::CCNode::create();
            utils::applyID(this, node);

            if (!utils::applyChildrenBuilder(this, node)) {
                (void)utils::applyChildren(this, node);
            }
            
            switch (this->axis) {
                case Axis::Vertical:
                    node->setLayout(
                        impl::ColumnGridLayout::create(
                            this->verticalDirection, this->horizontalDirection,
                            this->mainAxisSpacing, this->crossAxisSpacing,
                            this->crossAxisCount, this->childAspectRatio
                        )
                    );
                    break;
                case Axis::Horizontal:
                    node->setLayout(
                        impl::RowGridLayout::create(
                            this->horizontalDirection, this->verticalDirection,
                            this->mainAxisSpacing, this->crossAxisSpacing, 
                            this->crossAxisCount, this->childAspectRatio
                        )
                    );
                    break;
            }

            delete this;
            return node;
        }
    };
}