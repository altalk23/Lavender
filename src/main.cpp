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
				// .child = new ui::Grid {
				// 	.id = "my-column"_spr,
				// 	.mainAxisSpacing = 10,
				// 	.crossAxisSpacing = 10,
				// 	.childAspectRatio = 5,
				// 	.axis = ui::Axis::Horizontal,
				// 	.children = {
				// 		new ui::LayerColor {
				// 			.id = "my-cell-1"_spr,
				// 			.color = ccc4(118, 50, 100, 255),
				// 			.child = new ui::Container {
				// 				.id = "my-container"_spr,
				// 				.padding = ui::EdgeInsets::Symmetric { .vertical = 20, .horizontal = 10 },
				// 				.child = new ui::LayerColor {
				// 					.id = "my-cell-2-1"_spr,
				// 					.color = ccc4(255, 0, 187, 255),
				// 					.size = cocos2d::CCSize { 100, 100 },
				// 				},
				// 			},
				// 		},
				// 		new ui::LayerColor {
				// 			.id = "my-cell-2"_spr,
				// 			.color = ccc4(136, 93, 124, 255),
				// 		},
				// 		new ui::LayerColor {
				// 			.id = "my-cell-3"_spr,
				// 			.color = ccc4(46, 37, 116, 255),
				// 		},
				// 	},
				// },
				.child = new ui::Center {
					.child = new ui::Row {
						.id = "my-grid"_spr,
						.children = {
							new ui::Flexible {
								.child = new ui::LayerColor {
									.id = "my-cell-1-1"_spr,
									.color = ccc4(66, 60, 144, 255),
									.size = cocos2d::CCSize { 100, 30 },
								},	
							},
							new ui::Expanded {
								.child = new ui::LayerColor {
									.id = "my-cell-1-1"_spr,
									.color = ccc4(25, 129, 15, 255),
									.size = cocos2d::CCSize { 100, 100 },
								},	
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