#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Axis.hpp"

namespace ui {
    struct Sprite : public BaseInitializer<Sprite> {
        UICONSTRUCTOR_ADD_ID();
        UICONSTRUCTOR_ADD_CHILD();
        UICONSTRUCTOR_ADD_COPY_SIZE();
        UICONSTRUCTOR_ADD_SCALE();
        UICONSTRUCTOR_ADD_ROTATION();

        std::optional<std::string> fileName;
        std::optional<std::string> frameName;

        cocos2d::CCNode* construct() {
            cocos2d::CCSprite* node;

            if (fileName.has_value()) {
                node = cocos2d::CCSprite::create(fileName.value().c_str());
            }
            else if (frameName.has_value()) {
                node = cocos2d::CCSprite::createWithSpriteFrameName(frameName.value().c_str());
            }
            else {
                delete this;
                return nullptr;
            }

            utils::applyID(this, node);
            utils::applyScale(this, node);
            utils::applyRotation(this, node);

            auto wrapper = utils::generateWrapper(node);

            if (auto child = utils::applyChild(this, wrapper)) {
                utils::applyCopySize(this, wrapper, child);
            }

            utils::updateCopySize(this, wrapper);

            delete this;
            return wrapper;
        }
    };
}