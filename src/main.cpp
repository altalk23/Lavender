#include <Geode/Geode.hpp>

#include <UIConstructor.hpp>

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
						new ui::LayerColor {
							.id = "my-cell-1"_spr,
							.color = ccc4(255, 0, 0, 255),
							.width = 50,
						},
						new ui::Container {
							.id = "my-container"_spr,
							.padding = ui::EdgeInsets::Symmetric { .vertical = 20, .horizontal = 10 },
							.width = 80,
							.child = new ui::LayerColor {
								.id = "my-cell-2-1"_spr,
								.color = ccc4(255, 0, 187, 255),
							},
						},
						new ui::Expanded {
							.id = "my-cell-3"_spr,
							.flex = 2,
							.child = new ui::Column {
								.id = "my-column"_spr,
								.crossAxis = ui::CrossAxisAlignment::Stretch,
								.direction = ui::VerticalDirection::TopToBottom,
								.children = {
									new ui::LayerColor {
										.id = "my-cell-3-1"_spr,
										.color = ccc4(0, 255, 0, 255),
										.height = 100,
									},
									new ui::LayerColor {
										.id = "my-cell-3-2"_spr,
										.color = ccc4(255, 255, 0, 255),
										.height = 100,
									},
								},
							},
						},
						new ui::LayerColor {
							.id = "my-cell-4"_spr,
							.color = ccc4(0, 0, 255, 255),
							.width = 50,
						},
					}
				},
			},
		};

		auto node = gen->construct();

		CCDirector::sharedDirector()->replaceScene(static_cast<CCScene*>(node));
	}
};