#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"
#include "Expanded.hpp"

namespace ui {
    enum class MainAxisAlignment {
        // Align items to the start
        // |ooo......|
        Start,
        // All items are centered
        // |...ooo...|
        Center,
        // Align items to the end
        // |......ooo|
        End,
        // Each item gets the same portion from the layout (disregards gap)
        // |.o..o..o.|
        Even,
        // Space between each item is the same (disregards gap)
        // |o...o...o|
        Between,
        // Space around each item is the same (disregards gap)
        // |.o..o..o.|
        Around,
    };

    enum class CrossAxisAlignment {
        // Align items to the start
        // |ooo......|
        Start,
        // All items are centered
        // |...ooo...|
        Center,
        // Align items to the end
        // |......ooo|
        End,
        // Align items to the stretch
        // |ooooooooo|
        Stretch,
    };

    enum class VerticalDirection {
        // Items are laid out from top to bottom
        TopToBottom,
        // Items are laid out from bottom to top
        BottomToTop,
    };

    enum class HorizontalDirection {
        // Items are laid out from left to right
        LeftToRight,
        // Items are laid out from right to left
        RightToLeft,
    };

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
                auto ret = new ColumnLayout();
                ret->m_mainAxis = mainAxis;
                ret->m_crossAxis = crossAxis;
                ret->m_direction = direction;
                ret->autorelease();
                return ret;
            }
            virtual ~ColumnLayout() = default;

            void apply(cocos2d::CCNode* in) override {
                float totalHeight = 0.0f;
                float totalWidth = 0.0f;

                size_t flexCount = 0;

                for (auto child : geode::cocos::CCArrayExt<cocos2d::CCNode*>(in->getChildren())) {
                    if (auto expanded = geode::cast::typeinfo_cast<Expanded*>(child)) {
                        flexCount += expanded->m_flex;
                    }
                    else {
                        totalHeight += this->getNodeHeight(child);
                    }
                    totalWidth = std::max(totalWidth, this->getNodeWidth(child));
                }

                float gap = this->getNodeHeight(in) - totalHeight;
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

                for (size_t i = 0; i < in->getChildrenCount(); i++) {
                    auto child = static_cast<cocos2d::CCNode*>(in->getChildren()->objectAtIndex(i));

                    
                    auto center = this->getNodeSize(child) / 2.0f;
                    if (m_direction == VerticalDirection::TopToBottom) {
                        center.height = -center.height;
                    }
                    auto const endWidth = this->getNodeWidth(in) - this->getNodeWidth(child);
                    switch (m_crossAxis) {
                        case CrossAxisAlignment::Start:
                            this->setNodePosition(child, ccp(0, offset) + center);
                            break;
                        case CrossAxisAlignment::Center:
                            this->setNodePosition(child, ccp(endWidth / 2.0f, offset) + center);
                            break;
                        case CrossAxisAlignment::End:
                            this->setNodePosition(child, ccp(endWidth, offset) + center);
                            break;
                        case CrossAxisAlignment::Stretch:
                            this->setContentWidth(child, this->getNodeWidth(in));
                            this->setNodePosition(child, ccp(0, offset) + center);
                            break;
                    }
                    if (auto expanded = geode::cast::typeinfo_cast<Expanded*>(child)) {
                        this->setContentHeight(child, expanded->m_flex * flexGap);
                    }
                    child->ignoreAnchorPointForPosition(false);
                    child->setAnchorPoint(ccp(0.5f, 0.5f));
                    child->updateLayout();

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
                auto ret = new RowLayout();
                ret->m_mainAxis = mainAxis;
                ret->m_crossAxis = crossAxis;
                ret->m_direction = direction == HorizontalDirection::LeftToRight ? VerticalDirection::BottomToTop : VerticalDirection::TopToBottom;
                ret->autorelease();
                return ret;
            }
            virtual ~RowLayout() = default;

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
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILDREN();
        UICONSTRUCTOR_ADD_CHILDREN_BUILDER();

        MainAxisAlignment mainAxis = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxis = CrossAxisAlignment::Start;
        VerticalDirection direction = VerticalDirection::TopToBottom;

        cocos2d::CCNode* construct() {
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
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILDREN();
        UICONSTRUCTOR_ADD_CHILDREN_BUILDER();

        MainAxisAlignment mainAxis = MainAxisAlignment::Start;
        CrossAxisAlignment crossAxis = CrossAxisAlignment::Start;
        HorizontalDirection direction = HorizontalDirection::LeftToRight;

        cocos2d::CCNode* construct() {
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