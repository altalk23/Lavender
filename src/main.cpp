#include <Geode/Geode.hpp>

#include <Lavender.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		log::debug("Hello from my MenuLayer::init hook! This layer has {} children.", this->getChildrenCount());

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		auto gen = new ui::Scene {
			.id = "my-scene"_spr,
			.child = new ui::LayerColor {
				.id = "my-layer"_spr,
				.color = ccc4(8, 85, 71, 255),
				.keyBackClicked = [](){
					CCDirector::sharedDirector()->replaceScene(MenuLayer::scene(false));
				},
				.child = new ui::Row {
					.id = "my-grid"_spr,
					.crossAxis = ui::CrossAxisAlignment::Stretch,
					.children = {
						new ui::Expanded {
							.child = new ui::Align {
								.id = "my-align"_spr,
								.alignment = ui::Alignment::TopLeft,
								.child = new ui::LayerColor {
									.id = "my-cell-1-1"_spr,
									.color = ccc4(255, 0, 0, 255),
									.size = cocos2d::CCSize { 100, 100 },
								},	
							},
						},
						new ui::Container {
							.id = "my-container"_spr,
							.padding = ui::EdgeInsets::Symmetric { .vertical = 20, .horizontal = 10 },
							.width = 200,
							.child = new ui::LayerColor {
								.id = "my-cell-2-1"_spr,
								.color = ccc4(255, 0, 187, 255),
							},
						},
					},
				},
			},
		};

		auto node = gen->get();

		CCDirector::sharedDirector()->replaceScene(static_cast<CCScene*>(node));
	}
};