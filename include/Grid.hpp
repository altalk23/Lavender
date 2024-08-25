#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    enum class Axis : int {
        Horizontal,
        Vertical
    };

    namespace impl {
        class ColumnGridLayout : public cocos2d::Layout {
        public:
            VerticalDirection m_direction;
            HorizontalDirection m_crossDirection;
            float m_mainAxisSpacing;
            float m_crossAxisSpacing;
            size_t m_crossAxisCount;

            static ColumnGridLayout* create(
                VerticalDirection direction,
                HorizontalDirection crossDirection,
                float mainAxisSpacing,
                float crossAxisSpacing,
                size_t crossAxisCount
            ) {
                auto ret = new ColumnGridLayout();
                ret->m_direction = direction;
                ret->m_crossDirection = crossDirection;
                ret->m_mainAxisSpacing = mainAxisSpacing;
                ret->m_crossAxisSpacing = crossAxisSpacing;
                ret->m_crossAxisCount = crossAxisCount;
                ret->autorelease();
                return ret;
            }
            virtual ~ColumnGridLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                float crossAxisSpace = this->getNodeWidth(in) - (m_crossAxisCount - 1) * m_crossAxisSpacing;
                float singleCrossAxisSpace = crossAxisSpace / m_crossAxisCount;

                std::vector<float> mainAxisOffsets(m_crossAxisCount, 0.0f);
                float crossAxisOffset = 0.0f;

                for (size_t i = 0; i < in->getChildrenCount(); i++) {
                    auto child = static_cast<cocos2d::CCNode*>(in->getChildren()->objectAtIndex(i));

                    this->setContentWidth(child, singleCrossAxisSpace);

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
                    child->updateLayout();

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
                size_t crossAxisCount
            ) {
                auto ret = new RowGridLayout();
                ret->m_direction = direction == HorizontalDirection::LeftToRight ? VerticalDirection::BottomToTop : VerticalDirection::TopToBottom;
                ret->m_crossDirection = crossDirection == VerticalDirection::TopToBottom ? HorizontalDirection::RightToLeft : HorizontalDirection::LeftToRight;
                ret->m_mainAxisSpacing = mainAxisSpacing;
                ret->m_crossAxisSpacing = crossAxisSpacing;
                ret->m_crossAxisCount = crossAxisCount;
                ret->autorelease();
                return ret;
            }

            virtual ~RowGridLayout() = default;

            cocos2d::CCSize getNodeSize(cocos2d::CCNode* node) const override {
                return cocos2d::CCSize(node->getContentSize().height, node->getContentSize().width);
            }

            void setContentWidth(cocos2d::CCNode* node, float width) const override {
                node->setContentHeight(width);
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

        cocos2d::CCNode* construct() {
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
                            this->mainAxisSpacing, this->crossAxisSpacing, this->crossAxisCount
                        )
                    );
                    break;
                case Axis::Horizontal:
                    node->setLayout(
                        impl::RowGridLayout::create(
                            this->horizontalDirection, this->verticalDirection,
                            this->mainAxisSpacing, this->crossAxisSpacing, this->crossAxisCount
                        )
                    );
                    break;
            }

            delete this;
            return node;
        }
    };
}