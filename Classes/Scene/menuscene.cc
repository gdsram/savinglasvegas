/*! \brief This file give the functionality to MenuScene class.
    \file menuscene.cc
    \author Copyright owners
    \date 2/3/2014

    \copyright

    \attention <h1><center><strong>COPYRIGHT &copy; 2015 </strong>
    [<strong>Alvaro Denis</strong>][DENISACOSTAQ-URL] &
    [<strong>Luis Teijon</strong>][GDSRAM-URL]. All rights reserved.</center></h1>
    \attention This file is part of [<strong>Saving Las Vegas Project</strong>][SAVINGLASVEGAS-URL].

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    [DENISACOSTAQ-URL]: https://about.me/denisacostaq/ "Software Developer."
    [GDSRAM-URL]: https://about.me/gdsram/ "Software Developer."
    [SAVINGLASVEGAS-URL]: https://savinglasvegas.com/ "A cocos2d-x based game."
 */

#include "Classes/Scene/menuscene.h"
MenuScene::MenuScene()
  : restart_music_{false}
  , mvisible_size_ {CC::Size::ZERO}
  , visible_origin_ {CC::Vec2::ZERO}
  , bg_music_{"MainMenu/MainMenuMusic.ogg"}
  , heigth_score_{nullptr}
  , clouds_{nullptr}
  , credits_{nullptr}
  , menu_bg_{nullptr}
  , bg_{nullptr} {
}

MenuScene::~MenuScene() {
}

CC::Scene *MenuScene::scene() {
  CC::Scene *scene{CC::Scene::create()};
  scene->addChild(MenuScene::create());
  return scene;
}

bool MenuScene::init() {
  if (!Layer::init()) {
    return false;
  }

  mvisible_size_ = CC_DIRECTOR_->getVisibleSize();
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();

  bg_ = CC::Sprite::create("menu_background.png");
  this->addChild(bg_);
  bg_->setPosition(
     CC::Vec2 {mvisible_size_.width*.5f, mvisible_size_.height*.5f} +
       visible_origin_);

  // create clouds pool
  clouds_ = Clouds::create();
  clouds_->retain();
  for (Cloud *cloud : clouds_->clouds()) {
    this->addChild(cloud);
    for (CC::Sprite *fragment : cloud->fragments()) {
      this->addChild(fragment);
    }
  }
  clouds_->setToInitialState();
  for (int i{0}; i < clouds_->clouds().size() && i < 4; ++i) {
    clouds_->clouds().at(i)->launch(mvisible_size_.width * ((4 * i)/10.f),
                                    visible_origin_.y);
  }
  this->schedule(schedule_selector(MenuScene::updateLaunchCloud), 23.f);

  createMenu();
  createCredits();

  CC::EventListenerKeyboard *listener{CC::EventListenerKeyboard::create()};
  listener->onKeyReleased = ([this](CC::EventKeyboard::KeyCode keyCode,
                                   CC::Event* event) {
    if (keyCode == CC::EventKeyboard::KeyCode::KEY_BACK) {
      if (this->menu_bg_->isVisible() == true) {
        this->menuExitCallback();
      } else if (this->credits_->isVisible() == true) {
        this->showMenuCallback();
      } else {
        CC::log("BUG\n");
      }
    }
  });

  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener, this);
  return true;
}

void MenuScene::createMenu() noexcept {
  menu_bg_ = CC::Sprite::create("main_menu_background.png");
  this->addChild(menu_bg_, static_cast<int>(zOrder::zIndex_5));
  menu_bg_->setPosition(
     CC::Vec2 {mvisible_size_.width*.5f, mvisible_size_.height*.5f} +
       visible_origin_);

  /**************************************************************************/
  /*                                 Buttons                                */
  /**************************************************************************/
  CC::Vector<CC::MenuItem*> pMenuItems{};
  CC::MenuItemImage *pPlayItem{
    CC::MenuItemImage::create(
       "Buttons/play.png",
       "Buttons/play_pressed.png",
       CC_CALLBACK_0(MenuScene::menuPlayCallback, this))};

  pPlayItem->setPosition(
        CC::Vec2{menu_bg_->getContentSize().width*.275f,
                 menu_bg_->getContentSize().height*.725f});
  pMenuItems.pushBack(pPlayItem);

  CC::Sprite *audioButton{nullptr};
  if (G_COLABORATION_->audio_enabled()) {
    audioButton = CC::Sprite::create("Buttons/audio_on.png");
  } else {
    audioButton = CC::Sprite::create("Buttons/audio_off.png");
  }
  menu_bg_->addChild(audioButton);
  audioButton->setPosition(
        CC::Vec2{menu_bg_->getContentSize().width*.725f,
                 menu_bg_->getContentSize().height*.725f});
  CC::EventListenerTouchOneByOne* audioListener{
    CC::EventListenerTouchOneByOne::create()};
  audioListener->setSwallowTouches(true);
  audioListener->onTouchBegan = ([](CC::Touch* touch, CC::Event* event) {
    CC::Sprite *target{static_cast<CC::Sprite*>(event->getCurrentTarget())};
    CC::Size targetSize{target->getContentSize()};
    CC::Vec2 locationInNode{target->convertToNodeSpace(touch->getLocation())};
    CC::Rect rect{CC::Rect{0, 0, targetSize.width, targetSize.height}};
    if (rect.containsPoint(locationInNode)) {
      target->setOpacity(180);
      return true;
    } else {
      return false;
    }
  });
  audioListener->onTouchEnded = ([this, audioButton](CC::Touch* touch,
                                                     CC::Event* event){
      CC::Sprite *target = static_cast<CC::Sprite*>(event->getCurrentTarget());
      target->setOpacity(255);
      if (G_COLABORATION_->audio_enabled()) {
        CC_AUDIO_->pauseBackgroundMusic();
        CC_AUDIO_->stopAllEffects();
      } else {
        if (CC_AUDIO_->isBackgroundMusicPlaying()) {
          CC_AUDIO_->resumeBackgroundMusic();
        } else {
          CC_AUDIO_->playBackgroundMusic(bg_music_.c_str(), true);
        }
      }
      G_COLABORATION_->set_audio_enabled(!G_COLABORATION_->audio_enabled());
      if (G_COLABORATION_->audio_enabled()) {
        audioButton->setSpriteFrame(
              CC::Sprite::create("Buttons/audio_on.png")->getSpriteFrame());
      } else {
        audioButton->setSpriteFrame(
              CC::Sprite::create("Buttons/audio_off.png")->getSpriteFrame());
      }
  });  // NOLINT(readability/braces)
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        audioListener, audioButton);

#ifdef EVALUATION_VERSION
  CC::MenuItemImage *pCreditsItem{
    CC::MenuItemImage::create(
          std::string{"Buttons/short_credits.png"},
          std::string{"Buttons/short_credits_pressed.png"},
          CC_CALLBACK_0(MenuScene::showCreditsCallback, this))};
  pCreditsItem->setPosition(
        CC::Vec2{m_menu_bg->getContentSize().width*.275f,
                 m_menu_bg->getContentSize().height*.275f});

  CC::Sprite *download{
    CC::Sprite::create("Buttons/download.png")};
  m_menu_bg->addChild(download);
  download->setPosition(
        CC::Vec2{m_menu_bg->getContentSize().width*.725f,
                 m_menu_bg->getContentSize().height*.275f});
  CC::EventListenerTouchOneByOne* listener{
    CC::EventListenerTouchOneByOne::create()};
  listener->setSwallowTouches(true);
  listener->onTouchBegan = ([](CC::Touch* touch, CC::Event* event) {
    CC::Sprite *target{static_cast<CC::Sprite*>(event->getCurrentTarget())};
    CC::Size targetSize{target->getContentSize()};
    CC::Vec2 locationInNode{target->convertToNodeSpace(touch->getLocation())};
    CC::Rect rect{CC::Rect{0, 0, targetSize.width, targetSize.height}};
    if (rect.containsPoint(locationInNode)) {
      target->setOpacity(180);
      return true;
    } else {
      return false;
    }
  });
  listener->onTouchEnded = ([](CC::Touch* touch, CC::Event* event) {
      CC::Sprite *target{static_cast<CC::Sprite*>(event->getCurrentTarget())};
      CC::log("open URL");
      CC::log("open URL");
      // GlobalColaborationNP::openURL("url");
      target->setOpacity(255);
      // target->pause();
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener, download);
#else
  std::string lang{G_COLABORATION_->langueageSufix()};
  CC::MenuItemImage *pCreditsItem{
    CC::MenuItemImage::create(
          std::string{"Buttons/credits"} + lang + std::string{".png"},
          std::string{"Buttons/credits_pressed"} + lang + std::string{".png"},
          CC_CALLBACK_0(MenuScene::showCreditsCallback, this))};
  pCreditsItem->setPosition(
        CC::Vec2{menu_bg_->getContentSize().width*.5f,
                 menu_bg_->getContentSize().height*.275f});
#endif
  pMenuItems.pushBack(pCreditsItem);
  CC::Menu *m_menu{CC::Menu::createWithArray(pMenuItems)};
  m_menu->setPosition(CC::Vec2::ZERO);
  menu_bg_->addChild(m_menu, 3);

  CC::RepeatForever *blink{
    CC::RepeatForever::create(CC::Blink::create(.6f, 1))};
  CC::Sprite *record{CC::Sprite::create("record.png")};
  record->setPosition(
        CC::Vec2{bg_->getContentSize().width*.722f,
                 bg_->getContentSize().height*.42f});
  bg_->addChild(record);
  record->runAction(blink->clone());
  heigth_score_ = CC::Label::createWithTTF(
        "0",
        "fonts/Dosis-Light.otf",
        record->getContentSize().height*.7f);
  heigth_score_->setPosition(
        CC::Vec2{bg_->getContentSize().width*.722f,
                 bg_->getContentSize().height*.375f});
  heigth_score_->setRotation(-3.f);
  heigth_score_->runAction(blink->clone());
  bg_->addChild(heigth_score_);
}

void MenuScene::createCredits() noexcept {
  credits_ = CC::Sprite::create(
        std::string {"credits_body"} +
          G_COLABORATION_->langueageSufix() + std::string{".png"});
  this->addChild(credits_);
  credits_->setPosition(
        CC::Vec2 {mvisible_size_.width*.5f, mvisible_size_.height*.5f} +
          visible_origin_);
  credits_->setVisible(false);
  CC::MenuItemImage *button{
    CC::MenuItemImage::create(
       "Buttons/go_back.png",
       "Buttons/go_back_pressed.png",
       CC_CALLBACK_0(MenuScene::showMenuCallback, this))};
  button->setPosition(CC::Vec2{credits_->getContentSize().width*.5f,
                               credits_->getContentSize().height*.2f});
  CC::Menu *m{CC::Menu::createWithItem(button)};
  m->setPosition(CC::Vec2::ZERO);
  credits_->addChild(m);
}

void MenuScene::menuPlayCallback() noexcept {
  if (G_COLABORATION_->audio_enabled()) {
    if (CC_AUDIO_->isBackgroundMusicPlaying()) {
      CC_AUDIO_->pauseBackgroundMusic();
      CC_AUDIO_->stopAllEffects();
    }
  }
  CC_DIRECTOR_->pushScene(
        CC::TransitionFade::create(
          1.3f,
          GameLayer::getInstance()));
  restart_music_ = true;
}

void MenuScene::menuExitCallback() noexcept {
  if (G_COLABORATION_->audio_enabled()) {
    if (CC_AUDIO_->isBackgroundMusicPlaying()) {
      CC_AUDIO_->stopBackgroundMusic(true);
    }
    CC_AUDIO_->stopAllEffects();
    CC_AUDIO_->end();
  }
  CC_DIRECTOR_->popScene();
  CC_DIRECTOR_->end();
  GameLayer::destroyInstance();
  CC::Configuration::destroyInstance();
}

void MenuScene::onEnter() {
  Layer::onEnter();
  CC_AUDIO_->stopAllEffects();
  if (G_COLABORATION_->audio_enabled()) {
    CC_AUDIO_->playBackgroundMusic(bg_music_.c_str(), true);
  } else {
     // BUG(denisacostaq@gmail.com): Esto no deberia hacer falta.
    CC_AUDIO_->pauseBackgroundMusic();
  }
  char str[20]{0};
  std::snprintf(str, sizeof(str), "%u", G_COLABORATION_->height_score());
  heigth_score_->setString(std::string{str});
  AdmobHelper::showAd();
}
