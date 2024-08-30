#pragma once

#include <Geode/Geode.hpp>

#include "Base.hpp"
#include "Axis.hpp"

namespace ui {
    struct Sprite : public BaseInitializer<Sprite> {
        LAVENDER_ADD_ID();
        LAVENDER_ADD_SCALE();
        LAVENDER_ADD_ROTATION();

        std::optional<std::string> fileName;
        std::optional<std::string> frameName;

        LAVENDER_ADD_CHILD();

        cocos2d::CCNode* construct() const {
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

           (void)utils::applyChild(this, wrapper);
            utils::applySingleConstrainedLayout(this, wrapper);

            delete this;
            return wrapper;
        }
    };
}