#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Dark Souls-style death overlay with fade animation
class $modify(DeathOverlayPlayLayer, PlayLayer) {
	struct Fields {
		CCLayerColor* overlay = nullptr;
		CCLabelBMFont* label = nullptr;
		bool isProcessingDeath = false;
	};

	void resetLevel() {
		// Skip re-entry when already processing death
		if (m_fields->isProcessingDeath) {
			PlayLayer::resetLevel();
			return;
		}

		// Only show overlay on actual death (not practice mode)
		if (!m_player1 || !m_player1->m_isDead || m_isPracticeMode) {
			PlayLayer::resetLevel();
			return;
		}

		m_fields->isProcessingDeath = true;

		// Create and setup overlay
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto overlay = CCLayerColor::create({0, 0, 0, 0}, winSize.width, winSize.height);
		overlay->setAnchorPoint({0.f, 0.f});
		overlay->setPosition({0.f, 0.f});

		// Create and setup label
		auto label = CCLabelBMFont::create("YOU DIED", "bigFont.fnt");
		label->setScale(1.2f);
		label->setOpacity(0);
		label->setColor({255, 64, 64});
		label->setPosition(winSize.width / 2, winSize.height / 2);

		// Add to scene
		if (auto scene = CCDirector::sharedDirector()->getRunningScene()) {
			scene->addChild(overlay, 1000);
			scene->addChild(label, 1001);
		}

		m_fields->overlay = overlay;
		m_fields->label = label;

		// Play sound and animate
		FMODAudioEngine::sharedEngine()->playEffect("resources/audio/you_died.ogg"_spr);
		
		overlay->runAction(CCSequence::create(
			CCFadeTo::create(0.6f, 255),
			CCDelayTime::create(1.4f),
			CCCallFunc::create(this, callfunc_selector(DeathOverlayPlayLayer::performReset)),
			nullptr
		));
		label->runAction(CCFadeIn::create(0.6f));
	}

	void performReset() {
		// Clean up nodes
		if (m_fields->overlay) {
			m_fields->overlay->removeFromParent();
			m_fields->overlay = nullptr;
		}
		if (m_fields->label) {
			m_fields->label->removeFromParent();
			m_fields->label = nullptr;
		}

		// Continue original reset and clear flag
		PlayLayer::resetLevel();
		m_fields->isProcessingDeath = false;
	}
};
