#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Utils.hpp"

namespace ui {
    struct Node : public BaseInitializer<Node> {
        LAVENDER_ADD_ID();

        std::optional<float> scale;
        std::optional<float> rotation;

        std::optional<cocos2d::CCNode*> node;
        
        using GeneratorType = std::function<cocos2d::CCNode*()>;
        std::optional<GeneratorType> generator;

        cocos2d::CCNode* construct() const {
            cocos2d::CCNode* node = nullptr;

            if (this->node.has_value()) {
                node = this->node.value();
            }
            else if (this->generator.has_value()) {
                node = this->generator.value()();
            }
            else {
                delete this;
                return nullptr;
            }

            utils::applyID(this, node);

            if (this->scale.has_value()) {
                node->setScale(this->scale.value());
            }

            if (this->rotation.has_value()) {
                node->setRotation(this->rotation.value());
            }

            auto wrapper = utils::generateWrapper(node);

            delete this;
            return wrapper;
        }
    };
}