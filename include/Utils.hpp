#pragma once

#include <Geode/Geode.hpp>
#include "CopySizeLayout.hpp"

namespace ui::utils {
    #define UICONSTRUCTOR_ADD_ID() \
        std::optional<std::string> id
    
    void applyID(auto const* data, cocos2d::CCNode* node) {
        if (data->id.has_value()) {
            node->setID(data->id.value().c_str());
        }
    }

    #define UICONSTRUCTOR_ADD_CHILDREN() \
        std::vector<Base*> children

    bool applyChildren(auto const* data, cocos2d::CCNode* node) {
        for (auto child : data->children) {
            auto childNode = child->construct();
            node->addChild(childNode);
        }
        return data->children.size() > 0;
    }

    #define UICONSTRUCTOR_ADD_CHILDREN_BUILDER() \
        std::function<Base*(size_t)> builder; \
        size_t count = 0

    bool applyChildrenBuilder(auto const* data, cocos2d::CCNode* node) {
        if (data->builder) {
            for (size_t i = 0; i < data->count; i++) {
                auto child = data->builder(i);
                auto childNode = child->construct();
                node->addChild(childNode);
            }
            return true;
        }
        return false;
    }

    #define UICONSTRUCTOR_ADD_CHILD() \
        Base* child

    cocos2d::CCNode* applyChild(auto const* data, cocos2d::CCNode* node) {
        if (data->child != nullptr) {
            auto childNode = data->child->construct();
            node->addChild(childNode);
            return childNode;
        }
        return nullptr;
    }

    #define UICONSTRUCTOR_ADD_SIZE() \
        std::optional<cocos2d::CCSize> size; \
        std::optional<float> width; \
        std::optional<float> height

    void applySize(auto const* data, cocos2d::CCNode* node) {
        if (data->width.has_value()) {
            node->setContentWidth(data->width.value());
        }
        if (data->height.has_value()) {
            node->setContentHeight(data->height.value());
        }
        if (data->size.has_value()) {
            node->setContentSize(data->size.value());
        }
    }

    #define UICONSTRUCTOR_ADD_COPY_SIZE() \
        bool copySize = true

    void applyCopySize(auto const* data, cocos2d::CCNode* node, cocos2d::CCNode* child) {
        if (data->copySize) {
            node->setLayout(
                impl::CopySizeLayout::create(child)
            );
        }
    }

    void updateCopySize(auto const* data, cocos2d::CCNode* node) {
        if (data->copySize) {
            node->updateLayout();
        }
    }

    #define UICONSTRUCTOR_ADD_SCALE() \
        std::optional<float> scaleX; \
        std::optional<float> scaleY; \
        std::optional<float> scale

    void applyScale(auto const* data, cocos2d::CCNode* node) {
        if (data->scaleX.has_value()) {
            node->setScaleX(data->scaleX.value());
        }
        if (data->scaleY.has_value()) {
            node->setScaleY(data->scaleY.value());
        }
        if (data->scale.has_value()) {
            node->setScale(data->scale.value());
        }
    }

    #define UICONSTRUCTOR_ADD_ROTATION() \
        std::optional<float> rotation

    void applyRotation(auto const* data, cocos2d::CCNode* node) {
        if (data->rotation.has_value()) {
            node->setRotation(data->rotation.value());
        }
    }

    cocos2d::CCNode* generateWrapper(cocos2d::CCNode* node) {
        auto wrapper = cocos2d::CCNode::create();
        wrapper->addChild(node);
        wrapper->setContentSize(node->boundingBox().size);
        wrapper->setAnchorPoint(cocos2d::CCPoint { 0.5, 0.5 });
        node->setPosition(cocos2d::CCPoint {
            wrapper->getContentSize().width / 2,
            wrapper->getContentSize().height / 2
        });
        return wrapper;
    }
}