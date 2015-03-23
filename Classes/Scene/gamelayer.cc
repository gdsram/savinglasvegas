/*! \brief This file have the implementation for GameLayer class
    \file gamelayer.cc
    \author Copyright owners
    \date 12/2/2014

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

#include "Classes/Scene/gamelayer.h"

GameLayer::GameLayer()
  : _weaponExplodedCallback{
      std::bind(&GameLayer::weaponExplodedCallback,
                this,
                std::placeholders::_1)}
  , increaseScoreFunction{std::bind(&GameLayer::increaseScore,
                                    this,
                                    std::placeholders::_1)}
  , asteroid_hint_{nullptr}
  , asteroids_{nullptr}
  , bg_music_{"background.ogg"}
  , calendar_{nullptr}
  , cloud_hint_{nullptr}
  , clouds_ {nullptr}
#ifdef EVALUATION_VERSION
  , _ev_hint {nullptr}
#endif
  , dance0_{nullptr}
  , dance1_{nullptr}
  , deceased_performed_callback_{nullptr}
  , gc_{nullptr}
  , game_over_sprite_{nullptr}
  , healt_{nullptr}
  , hints_actions_locket_{true}
  , lang_{"_us"}
  , menu_{nullptr}
  , menu_items_background_{nullptr}
  , menu_on_pause_{nullptr}
  , paralax_hint_{nullptr}
  , paralax_middle_node_{nullptr}
  , paralax_root_node_{nullptr}
  , packageid_hint_{nullptr}
  , packages_{nullptr}
  , weapon_manager_{nullptr}
  , visible_origin_{CC::Vec2::ZERO}
  , visible_size_{CC::Size::ZERO}
  , accurancy_label_{nullptr}
  , are_touching_{false}
  , asteroids_laucher_interval_{7.5f}
  , brocken_asteroids_with_boms_{0u}
  , count_rltzt_{_initialCountRltzt}
  , count_rltzt_acum_{0}
  , clouds_laucher_interval_{19.f}
  , days_go_label_{nullptr}
  , exploded_meteors_{0u}
  , gameover_info_{nullptr}
  , heigth_score_{nullptr}
  , packages_laucher_interval_{60.f}  // CORREGIR:
  , paralax_is_active_{false}
  // , m_rescheduleMeteors{false}  // FIXME
  , reverse_speed_{_initialReverseSpeed}
  , score_{0}
  , score_label_{nullptr}
  , score_go_label_{nullptr}
  , total_score_label_{nullptr}
  , used_bombs_{0u} {
}

GameLayer::~GameLayer() {
  CC_SAFE_RELEASE(weapon_manager_);

  CC_SAFE_RELEASE(calendar_);
  CC_SAFE_RELEASE(asteroids_);
  CC_SAFE_RELEASE(clouds_);
  CC_SAFE_RELEASE(dance0_);
  CC_SAFE_RELEASE(dance1_);
  CC_SAFE_RELEASE(deceased_performed_callback_);

  this->deleteSound();
}

CC::Scene* GameLayer::_selfInstance = nullptr;
CC::Scene* GameLayer::getInstance() noexcept {
  if (_selfInstance == nullptr) {
    _selfInstance = GameLayer::scene();
  }
  CCASSERT(_selfInstance,
           "FATAL: Not enough memory in GameLayer::getInstance()");
  return _selfInstance;
}

void GameLayer::destroyInstance() noexcept {
  CC_SAFE_RELEASE_NULL(_selfInstance);
}

void GameLayer::onEnter() {
  CC::Layer::onEnter();
  setToInitialState();
  if (G_COLABORATION_->audio_enabled()) {
    this->runAction(CC::Sequence::createWithTwoActions(
                      CC::DelayTime::create(1.5f),
                      CC::CallFunc::create([this]{
      CC_AUDIO_->playBackgroundMusic(bg_music_.c_str(), true);
    })));
  }
}

/******************************************************************************/
/*                                    INIT                                    */
/******************************************************************************/
bool GameLayer::init() {
  if (!Layer::init()) {
      return false;
  }
  createMemebers();
  createParalaxEfect();
  createWeaponSelector();
  createPools();
  createMenu();
  createGameOverLayer();
  createSound();

  CC::EventListenerKeyboard *keysListener{
    CC::EventListenerKeyboard::create()};
  keysListener->onKeyReleased =
      ([this](CC::EventKeyboard::KeyCode keyCode, CC::Event*) {
    if (!gc_->paused() &&
        keyCode == CC::EventKeyboard::KeyCode::KEY_BACK) {
      this->menuPauseCallback();
    }
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        keysListener, this);

  CC::EventListenerTouchOneByOne* hintsListener{
    CC::EventListenerTouchOneByOne::create()};
  hintsListener->setSwallowTouches(true);
  hintsListener->onTouchBegan = ([&](CC::Touch* touch, CC::Event* event) {
    CC::Node *target{event->getCurrentTarget()};
    if (target->getOpacity() >= 150) {
      CC_DIRECTOR_->getEventDispatcher()->pauseEventListenersForTarget(target);
      target->stopAllActions();
      target->setVisible(false);
      this->resumeGame();
      return true;
    } else {
      return false;
    }
  });
  for (CC::Node *hint :
        std::vector<CC::Node*>{asteroid_hint_, cloud_hint_, packageid_hint_}) {
    CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
          hintsListener->clone(), hint);
  }
  return true;
}

void GameLayer::createMemebers() noexcept {
  deceased_performed_callback_ =
      CC::CallFuncN::create(
        CC_CALLBACK_1(GameLayer::deceasedPerformedCallback, this));
  deceased_performed_callback_->retain();
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  visible_size_ = CC_DIRECTOR_->getVisibleSize();
  gc_ = G_COLABORATION_;
  lang_ = gc_->langueageSufix();

//  CC_SFC_->addSpriteFramesWithFile("IMG/sprite_sheet.plist");
//  this->addChild(CC::SpriteBatchNode::create("IMG/sprite_sheet.png"));
  CC::Sprite *healtBackground{
    CC::Sprite::create(
          "Animations/healt_progress_bar/healt_background.png")};
  healtBackground->setAnchorPoint(CC::Vec2::ANCHOR_TOP_LEFT);
  this->addChild(healtBackground, static_cast<int>(zOrder::zIndex_6));
  healt_ = Healt::create();
  healt_->set_healt_background(healtBackground);
  this->addChild(healt_, static_cast<int>(zOrder::zIndex_6));
  healtBackground->setPosition(healt_->getPosition());

  calendar_ = Calendar::create();
  calendar_->retain();
  this->addChild(const_cast<CC::LayerColor*>(calendar_->nigth()),
                 static_cast<int>(zOrder::zIndex_5));

  // TOUCH
  auto dispacher = CC_DIRECTOR_->getEventDispatcher();
  CC::EventListenerTouchOneByOne *listener{
    CC::EventListenerTouchOneByOne::create()};
  listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
  listener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
  listener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
  dispacher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameLayer::createMenu() noexcept {
  this->addChild(const_cast<CC::Sprite*>(calendar_->passed_days_background()),
                static_cast<int>(zOrder::zIndex_6));
  calendar_->setScoreLabel((score_label_ = CC::Label::createWithTTF(
        "0",
        "fonts/Dosis-Light.otf",
        visible_size_.height*.045f)));
  score_label_->setScaleX(
        CC_DIRECTOR_->getOpenGLView()->getFrameSize().width/
        CC_DIRECTOR_->getOpenGLView()->getDesignResolutionSize().width);
  CC::MenuItemImage *pPauseItem{
    CC::MenuItemImage::create(
       "Buttons/pause.png",
       "Buttons/pause_pressed.png",
       CC_CALLBACK_0(GameLayer::menuPauseCallback, this))};
  pPauseItem->setPosition(
        CC::Vec2{
      visible_size_.width - pPauseItem->getContentSize().width*.5f,
      visible_size_.height - pPauseItem->getContentSize().height*.5f});

  menu_ = CC::Menu::createWithItem(pPauseItem);
  menu_->setPosition(CC::Vec2::ZERO + visible_origin_);
  this->addChild(menu_, static_cast<int>(zOrder::zIndex_6));

  menu_items_background_ = CC::Sprite::create(
        "on_pause_background.png");
  menu_items_background_->setVisible(false);
  menu_items_background_->setPosition(
        CC::Vec2 {visible_size_.width*.5f, visible_size_.height*.5f} +
          visible_origin_);
  this->addChild(menu_items_background_, static_cast<int>(zOrder::zIndex_6));

  /**********************************Buttons*********************************/
  CC::Vector<CC::MenuItem*> pMenuItems{};
  CC::MenuItemImage *pResumeItem{
    CC::MenuItemImage::create(
          "Buttons/play.png",
          "Buttons/play_pressed.png",
          CC_CALLBACK_0(GameLayer::menuResumeCallback, this))};
  pResumeItem->setPosition(
        CC::Vec2{menu_items_background_->getContentSize().width * .5f,
                 menu_items_background_->getContentSize().height * .83f});
  pMenuItems.pushBack(pResumeItem);

  CC::MenuItemImage *pRestartItem{
    CC::MenuItemImage::create(
       "Buttons/restart.png",
       "Buttons/restart_pressed.png",
       CC_CALLBACK_0(GameLayer::menuRestartCallback, this))};
  pRestartItem->setPosition(
        CC::Vec2{menu_items_background_->getContentSize().width * .5f,
                 menu_items_background_->getContentSize().height * .5f});
  pMenuItems.pushBack(pRestartItem);

  CC::MenuItemImage *pMainMenu{
    CC::MenuItemImage::create(
       "Buttons/main_menu.png",
       "Buttons/main_menu_pressed.png",
       CC_CALLBACK_0(GameLayer::menuExitCallback, this))};
  pMainMenu->setPosition(
        CC::Vec2{menu_items_background_->getContentSize().width * .5f,
                 menu_items_background_->getContentSize().height * .17f});
  pMenuItems.pushBack(pMainMenu);

  menu_on_pause_ = CC::Menu::createWithArray(pMenuItems);
  menu_on_pause_->setPosition(CC::Vec2::ZERO);
  menu_items_background_->addChild(menu_on_pause_,
                                   static_cast<int>(zOrder::zIndex_6));
  menu_on_pause_->setVisible(false);

#ifdef EVALUATION_VERSION
  std::string ls{GlobalColaboration::getInstance()->langueageSufix()};
  _ev_hint = CC::Sprite::create("end_hint" + ls + ".png");
  this->addChild(_ev_hint);
  _ev_hint->setPosition(
        CC::Vec2 {visible_size_.width*.5f, visible_size_.height*.5f} +
          visible_origin_);

  CC::Vector<CC::MenuItem*> pMenuPayItems{};
  CC::MenuItemImage *pPlayAgainPayItem{
    CC::MenuItemImage::create(
       "Buttons/game_over_restart.png",
       "Buttons/game_over_restart_pressed.png",
       CC_CALLBACK_0(GameLayer::menuRestartCallback, this))};
  pMenuPayItems.pushBack(pPlayAgainPayItem);
  CC::MenuItemImage *pMainMenuPayItem{
    CC::MenuItemImage::create(
       "Buttons/game_over_main_menu.png",
       "Buttons/game_over_main_menu_pressed.png",
       CC_CALLBACK_0(GameLayer::menuExitCallback, this))};
  pMenuPayItems.pushBack(pMainMenuPayItem);
  CC::MenuItemImage *pDownloadPayItem{
    CC::MenuItemImage::create(
          "Buttons/end_download.png",
          "Buttons/end_download_pressed.png",
          CC_CALLBACK_0(GameLayer::menuDownloadCallback, this))};
  pMenuPayItems.pushBack(pDownloadPayItem);
  CC::Menu *pMenuPay{CC::Menu::createWithArray(pMenuPayItems)};
  pMenuPay->setPosition(
        CC::Vec2{_ev_hint->getContentSize().width*.5f,
                 _ev_hint->getContentSize().height*.13f});
  pMenuPay->alignItemsHorizontallyWithPadding(
        _ev_hint->getContentSize().width*.03f);
  _ev_hint->addChild(pMenuPay);
#endif
}

void GameLayer::createGameOverLayer() noexcept {
  game_over_sprite_ = CC::Sprite::create(
          "background_game_over.png");
  game_over_sprite_->setPosition(
        CC::Vec2 {visible_size_.width*.5f, visible_size_.height*.5f} +
          visible_origin_);
  this->addChild(game_over_sprite_, static_cast<int>(zOrder::zIndex_6) + 5);

  gameover_info_ = CC::Sprite::create(
     std::string{"gameover_info"} + lang_ + std::string{".png"});
  gameover_info_->setPosition(
        CC::Vec2{game_over_sprite_->getContentSize().width*.5f,
                 game_over_sprite_->getContentSize().height*.5f});
  game_over_sprite_->addChild(gameover_info_,
                              static_cast<int>(zOrder::zIndex_6) + 6);

  const CC::Vector<CC::MenuItem*>& pMenuItems{};
  CC::MenuItemImage *pPlayAgainItem{
    CC::MenuItemImage::create(
       "Buttons/game_over_restart.png",
       "Buttons/game_over_restart_pressed.png",
       CC_CALLBACK_0(GameLayer::menuRestartCallback, this))};
  const_cast<CC::Vector<CC::MenuItem*>&>(pMenuItems).pushBack(pPlayAgainItem);
  CC::MenuItemImage *pExitItem{
    CC::MenuItemImage::create(
       "Buttons/game_over_main_menu.png",
       "Buttons/game_over_main_menu_pressed.png",
       CC_CALLBACK_0(GameLayer::menuExitCallback, this))};
  const_cast<CC::Vector<CC::MenuItem*>&>(pMenuItems).pushBack(pExitItem);
  CC::Menu *menu_game_over{CC::Menu::createWithArray(pMenuItems)};
  menu_game_over->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.5f,
                 gameover_info_->getContentSize().height*.13f});
  menu_game_over->alignItemsHorizontallyWithPadding(
        gameover_info_->getContentSize().width*.03f);
  gameover_info_->addChild(menu_game_over);

  days_go_label_ =
  CC::Label::createWithTTF("0",
                           "fonts/Dosis-Light.otf",
                           gameover_info_->getContentSize().height*.07f);
  days_go_label_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
  days_go_label_->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.58f,
                 gameover_info_->getContentSize().height*.82f});
  gameover_info_->addChild(days_go_label_);
  score_go_label_ =
    CC::Label::createWithTTF("0",
                             "fonts/Dosis-Light.otf",
                             gameover_info_->getContentSize().height*.07f);
  score_go_label_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
  score_go_label_->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.58f,
                 gameover_info_->getContentSize().height*.693f});
  gameover_info_->addChild(score_go_label_);
  accurancy_label_ =
    CC::Label::createWithTTF(std::string{"0"},
                             "fonts/Dosis-Light.otf",
                             gameover_info_->getContentSize().height*.07f);
  accurancy_label_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
  accurancy_label_->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.58f,
                 gameover_info_->getContentSize().height*.57f});
  gameover_info_->addChild(accurancy_label_);
  total_score_label_ =
    CC::Label::createWithTTF("0",
                             "fonts/Dosis-Light.otf",
                             gameover_info_->getContentSize().height*.07f);
  total_score_label_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
  total_score_label_->enableOutline(
        CC::Color4B{255, 255, 255, 255},
        total_score_label_->getContentSize().height*.05f);
  total_score_label_->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.58f,
                 gameover_info_->getContentSize().height*.445f});
  gameover_info_->addChild(total_score_label_);
  heigth_score_ =
      CC::Label::createWithTTF("0",
                               "fonts/Dosis-Light.otf",
                               gameover_info_->getContentSize().height*.07f);
  heigth_score_->setColor(CC::Color3B{204, 84, 0});
  heigth_score_->enableOutline(CC::Color4B{204, 84, 0, 255},
                              heigth_score_->getContentSize().height*.05f);
  heigth_score_->enableShadow(
        CC::Color4B{0, 0, 0, 200},
        CC::Size{heigth_score_->getContentSize().width*.09f,
                 -heigth_score_->getContentSize().height*.07f},
        10);
  heigth_score_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
  heigth_score_->setPosition(
        CC::Vec2{gameover_info_->getContentSize().width*.58f,
                 gameover_info_->getContentSize().height*.322f});
  gameover_info_->addChild(heigth_score_);

}

void GameLayer::createWeaponSelector() noexcept {
  weapon_manager_ = WeaponManager::create(
        this,
        schedule_selector(GameLayer::updateAtomicBomsCollitions),
        schedule_selector(GameLayer::updateBombsCollitions),
        schedule_selector(GameLayer::updateTornadosCollitions),
        _weaponExplodedCallback);
  weapon_manager_->retain();
  this->addChild(
        const_cast<CC::Sprite*>(weapon_manager_->bombWeaponSelector()),
        9);
  this->addChild(
        const_cast<CC::Sprite*>(weapon_manager_->atomicBombWeaponSelector()),
        9);
  this->addChild(
        const_cast<CC::Sprite*>(weapon_manager_->tornadoWeaponSelector()),
        9);
}

void GameLayer::createParalaxEfect() noexcept {
  paralax_root_node_ = CC::ParallaxNode::create();
  this->addChild(paralax_root_node_,
                 static_cast<int>(zOrder::zIndex_0));
  paralax_root_node_->setPosition(CC::Vec2::ZERO + visible_origin_);
  Node *mountain_back_node{Node::create()};
  paralax_root_node_->addChild(
        mountain_back_node,
        static_cast<int>(zOrderInParalax::zIndex_0),
        CC::Vec2{.7f, 1.0f},
        CC::Vec2::ZERO);
  mountain_back_node->setPosition(
        CC::Vec2{visible_size_.width*.5f, visible_size_.height*.5f});
  CC::Sprite *mountain_back{CC::Sprite::create(
          "mountain_back.png")};
  mountain_back->setPosition(
        CC::Vec2{visible_size_.width*.5f, visible_size_.height*.1f});
  mountain_back_node->addChild(const_cast<CC::Sprite*>(
                                    calendar_->sky()));
  mountain_back_node->addChild(const_cast<CC::Sprite*>(calendar_->stars()));
  mountain_back_node->addChild(mountain_back);

  CC::Node *mountain_front_node{Node::create()};
  mountain_front_node->setPosition(
        CC::Vec2{visible_size_.width*.5f, visible_size_.height*.5f});
  paralax_root_node_->addChild(
              mountain_front_node,
              static_cast<int>(zOrderInParalax::zIndex_1),
              CC::Vec2{.78f, 1.f},
              CC::Vec2::ZERO);

  CC::Sprite *mountain_front{CC::Sprite::create(
          "mountain_front.png")};
  mountain_front->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_BOTTOM);
  mountain_front->setPosition(CC::Vec2{visible_size_.width*.5f, 0.f});
  mountain_front_node->addChild(mountain_front);

  CC::Node *edifices_back_node{Node::create()};
  edifices_back_node->setPosition(
        CC::Vec2 {visible_size_.width*.5f, visible_size_.height*.5f} +
          visible_origin_);
  paralax_root_node_->addChild(
              edifices_back_node,
              static_cast<int>(zOrderInParalax::zIndex_1),
              CC::Vec2{.85f, 1.f},
              CC::Vec2::ZERO);

  paralax_middle_node_ = Node::create();
  paralax_root_node_->addChild(paralax_middle_node_,
                         static_cast<int>(zOrderInParalax::zIndex_2),
                         CC::Vec2{0.87f, 1.0f},
                         CC::Vec2::ZERO);
  paralax_middle_node_->addChild(const_cast<CC::Sprite*>(calendar_->sun()),
                               static_cast<int>(zOrder::zIndex_0));
  paralax_middle_node_->addChild(const_cast<CC::Sprite*>(calendar_->moon()),
                               static_cast<int>(zOrder::zIndex_0));

  CC::Sprite *edifices_back{CC::Sprite::create(
          "edifices_back.png")};
  edifices_back->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_BOTTOM);
  edifices_back->setPosition(
        CC::Vec2{visible_size_.width*.5f, 0.f});
  edifices_back_node->addChild(edifices_back);

  CC::Node *edif_frente_node{Node::create()};
  edif_frente_node->setPosition(
        CC::Vec2{visible_size_.width*.5f, visible_size_.height*.5f});
  paralax_root_node_->addChild(
              edif_frente_node,
              static_cast<int>(zOrderInParalax::zIndex_3),
              CC::Vec2{.9f, 1.f},
              CC::Vec2::ZERO);
  CC::Sprite *edif_frente{CC::Sprite::create(
          "edifices_front.png")};
  edif_frente->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_BOTTOM);
  edif_frente->setPosition(
        CC::Vec2{visible_size_.width*.5f, 0.f});
  edif_frente_node->addChild(edif_frente);

  CC::MoveBy *mv{
    CC::MoveBy::create(.3f, CC::Vec2{visible_size_.width*.03f, 0.f})};
  dance0_ = CC::Repeat::create(
        CC::Sequence::create(
          CC::CallFunc::create([this]{
    paralax_hint_->setVisible(true);
  }),
          mv->clone(),
          mv->reverse()->clone(),
          CC::CallFunc::create([this]{
    paralax_hint_->setVisible(false);
  }),
          nullptr), 3);
  dance0_->retain();
  dance1_ = CC::Repeat::create(
        CC::Sequence::create(mv->reverse()->clone(), mv->clone(), nullptr), 3);
  dance1_->retain();
}

void GameLayer::createPools() noexcept {
  asteroids_ = Asteroids::create();
  asteroids_->retain();
  for (auto meteor : asteroids_->meteors()) {
    paralax_middle_node_->addChild(meteor, static_cast<int>(zOrder::zIndex_1));
  }
  for (const auto& num : asteroids_->nums()) {
    paralax_middle_node_->addChild(num, static_cast<int>(zOrder::zIndex_6));
  }
  packages_ = Packages::create(
        std::bind(
          &GameLayer::deceasedPerformedCallback, this, std::placeholders::_1));
  packages_->retain();
  for (auto package : packages_->packages()) {
    paralax_middle_node_->addChild(package, static_cast<int>(zOrder::zIndex_3));
  }
  for (GenericWeapon *tornado : weapon_manager_->tornados()->weapons()) {
    paralax_middle_node_->addChild(tornado, static_cast<int>(zOrder::zIndex_2));
  }
  this->addChild(weapon_manager_->atomicBombs()->lightingReadAlarm(),
                 static_cast<int>(zOrder::zIndex_5));
  for (GenericWeapon* atomBom : weapon_manager_->atomicBombs()->weapons()) {
    paralax_middle_node_->addChild(atomBom, static_cast<int>(zOrder::zIndex_3));
  }
  for (GenericWeapon* bomb : weapon_manager_->bombs()->weapons()) {
    paralax_middle_node_->addChild(bomb, static_cast<int>(zOrder::zIndex_3));
    paralax_middle_node_->addChild(static_cast<Bomb*>(bomb)->medal(),
                                  static_cast<int>(zOrder::zIndex_3));
  }
  // create clouds pool
  clouds_ = Clouds::create();
  clouds_->retain();
  for (Cloud *cloud : clouds_->clouds()) {
    paralax_middle_node_->addChild(cloud, static_cast<int>(zOrder::zIndex_4));
    for (CC::Sprite *fragment : cloud->fragments()) {
      paralax_middle_node_->addChild(fragment,
                                     static_cast<int>(zOrder::zIndex_4));
    }
  }
  asteroid_hint_ = CC::Sprite::create(
     std::string{"asteroid_hint"} + lang_ + std::string{".png"});
  this->addChild(asteroid_hint_, static_cast<int>(zOrder::zIndex_6) + 5);
  asteroid_hint_->setPosition(
        CC::Vec2 {visible_size_.width*.5f,
                  visible_size_.height*.5f} + visible_origin_);
  cloud_hint_ = CC::Sprite::create(
     std::string{"cloud_hint"} + lang_ + std::string{".png"});
  this->addChild(cloud_hint_, static_cast<int>(zOrder::zIndex_6) + 5);
  cloud_hint_->setPosition(
        CC::Vec2 {visible_size_.width*.5f,
                  visible_size_.height*.5f} + visible_origin_);
  packageid_hint_ = CC::Sprite::create(
     std::string{"package_hint" + lang_ + ".png"});
  this->addChild(packageid_hint_, static_cast<int>(zOrder::zIndex_6));
  packageid_hint_->setPosition(
        CC::Vec2 {visible_size_.width*.5f,
                  visible_size_.height*.5f} + visible_origin_);
  paralax_hint_ = CC::Node::create();
  this->addChild(paralax_hint_, static_cast<int>(zOrder::zIndex_6) + 5);
  paralax_hint_->setPosition(
        CC::Vec2 {visible_size_.width*.5f,
                  visible_size_.height*.5f} + visible_origin_);
  CC::Sprite *arrowLeft{CC::Sprite::create("arrow_left.png")};
  paralax_hint_->addChild(arrowLeft);
  arrowLeft->setPositionX(visible_size_.width*.2f);
  CC::Sprite *arrowRigth{CC::Sprite::create("arrow_right.png")};
  arrowRigth->setPositionX(-visible_size_.width*.2f);
  paralax_hint_->addChild(arrowRigth);
}

void GameLayer::createSound() noexcept {
  if (G_COLABORATION_->audio_enabled()) {
    CC_AUDIO_->preloadBackgroundMusic(bg_music_.c_str());

    CC_AUDIO_->preloadEffect("alarm.ogg");
    CC_AUDIO_->preloadEffect("asteroid.ogg");
    CC_AUDIO_->preloadEffect("bomb.ogg");
    CC_AUDIO_->preloadEffect("medall.ogg");
    CC_AUDIO_->preloadEffect("package_taken.ogg");
    CC_AUDIO_->preloadEffect("MainMenu/show_credits.ogg");

    CC_AUDIO_->setBackgroundMusicVolume(
                G_COLABORATION_->bg_music_volume());
    CC_AUDIO_->setEffectsVolume(
                G_COLABORATION_->ef_music_volume());
  }
}

void GameLayer::setToInitialState() noexcept {
  // BUG(denisacostaq@gmail.com): ftm bug.
  if (!G_COLABORATION_->audio_enabled()) {
    CC_AUDIO_->stopBackgroundMusic(bg_music_.c_str());
  }
  for (CC::Node *hint :
        std::vector<CC::Node*>{asteroid_hint_, cloud_hint_, packageid_hint_}) {
    CC_DIRECTOR_->getEventDispatcher()->pauseEventListenersForTarget(hint);
  }
  gameover_info_->setVisible(false);
  game_over_sprite_->setVisible(false);
  asteroid_hint_->setOpacity(0);
  asteroid_hint_->setVisible(true);
  cloud_hint_->setOpacity(0);
  cloud_hint_->setVisible(true);
#ifdef EVALUATION_VERSION
  _ev_hint->setVisible(false);
  _ev_hint->getChildren().at(0)->resume();
#endif
  packageid_hint_->setOpacity(0);
  packageid_hint_->setVisible(true);
  paralax_hint_->setVisible(false);
  gc_->resume();
  menu_->setEnabled(true);
  menu_->setVisible(true);
  score_label_->setVisible(true);
  gc_->set_extra_width_factor(.0f);
  //  FIXME
  //  m_used_pckaget_count = 0;

  are_touching_ = false;
  asteroids_laucher_interval_ = 7.5f;
  brocken_asteroids_with_boms_ = 0u;
  clouds_laucher_interval_ = 19.f;
  packages_laucher_interval_ = 60.f;
  count_rltzt_ = _initialCountRltzt;
  count_rltzt_acum_ = 0;
  exploded_meteors_ = 0u;
  hints_actions_locket_ = true;
  paralax_is_active_ = false;
  paralax_root_node_->setPosition(CC::Vec2::ZERO + visible_origin_);
  reschedule_meteors_ = true;
  reverse_speed_ = _initialReverseSpeed;
  used_bombs_ = 0u;
  score_ = 0;
  this->increaseScore(0);

  // m_exploded_by_bomb = 0;
  asteroids_->setToInitialState();
  weapon_manager_->setToInitialState();
  calendar_->setToInitialState();
  clouds_->setToInitialState();
  packages_->setToInitialState();
  healt_->setToInitialState();

  initGameSchedulers();
  AdmobHelper::hideAd();
}

void GameLayer::initGameSchedulers() noexcept {
  this->schedule(schedule_selector(GameLayer::updateCurrentBombs),
                 50.f);
  this->scheduleOnce(schedule_selector(GameLayer::launchFirstAsteroidScheduled),
                     2.f);
  this->scheduleOnce(schedule_selector(GameLayer::launchFirstCloudScheduled),
                     clouds_laucher_interval_);
  this->scheduleOnce(schedule_selector(GameLayer::launchFirstPackageScheduled),
                     packages_laucher_interval_);
  this->schedule(schedule_selector(GameLayer::updateReverseSpeedInterval),
                 23.f);
  this->scheduleOnce(
         schedule_selector(GameLayer::automaticParalaxEfect), 180.f);
#ifdef EVALUATION_VERSION
  this->scheduleOnce(
        schedule_selector(GameLayer::evaluationVersionScheduled), 5.f);
#endif
}

#ifdef EVALUATION_VERSION
void GameLayer::evaluationVersionScheduled(float dt) {
  _menu->setVisible(false);
  _hintsActionsLocket = false;
  pauseGame();
  _ev_hint->setVisible(true);
}
#endif

void GameLayer::automaticParalaxEfect(float dt) noexcept {
  this->schedule(schedule_selector(GameLayer::onParalaxActivated),
                 16.f);
  if (gc_->show_hints() == true) {
    pauseGame();
    CC::Sequence *seq2{CC::Sequence::create(
        CC::MoveTo::create(
              3.5f,
              CC::Vec2(-.35f*visible_size_.width, 0.f) + visible_origin_),
        CC::MoveTo::create(
              7.f,
              CC::Vec2(.35f*visible_size_.width, 0.f) + visible_origin_),
        CC::MoveTo::create(
              3.5f,
              CC::Vec2 {CC::Vec2::ZERO.x, 0.f} + visible_origin_),
        CC::CallFunc::create([this](){
        this->resumeGame();
        paralax_is_active_ = true;
        this->updateExtraWidthFactor(0.f);
        this->schedule(schedule_selector(GameLayer::updateExtraWidthFactor),
                       6.f);
        gc_->set_show_hints(false);
      }),
        nullptr)};
    seq2->setTag(static_cast<int>(NodeTag::paralax));
    paralax_root_node_->runAction(seq2);
  } else {
    paralax_hint_->getChildren().at(0)->runAction(dance0_->clone());
    paralax_hint_->getChildren().at(1)->runAction(
          CC::Sequence::create(dance1_->clone(),
                               CC::CallFunc::create([this]{
    paralax_is_active_ = true;
    this->updateExtraWidthFactor(0.f);
    this->schedule(schedule_selector(GameLayer::updateExtraWidthFactor), 6.f);
    paralax_hint_->getChildren().at(0)->runAction(
          CC::RepeatForever::create(
            CC::Sequence::create(CC::DelayTime::create(15.f),
                                 dance0_->clone(),
                                 nullptr)));
    paralax_hint_->getChildren().at(1)->runAction(
          CC::RepeatForever::create(
            CC::Sequence::create(CC::DelayTime::create(15.f),
                                 dance1_->clone(),
                                 nullptr)));
  }),
                                           nullptr));
  }
}

void GameLayer::onParalaxActivated(float dt) noexcept {
  // NOTE(denisacostaq@gmail.com):
  // no se restea por initAtThe.. pro no importa.
  static bool switcH{true};
  if (switcH) {
    packages_->setPendantPackages(
          Package::Kind::AtomicBomb,
          packages_->pendantPackages(Package::Kind::AtomicBomb) + 1);
    packages_->launchNext(Package::Kind::AtomicBomb, reverse_speed_);
  } else {
    packages_->setPendantPackages(
          Package::Kind::Tornado,
          packages_->pendantPackages(Package::Kind::Tornado) + 2);
    packages_->launchNext(Package::Kind::Tornado, reverse_speed_);
    this->asteroidsRelentization();
  }
  switcH = !switcH;
}


/******************************************************************************/
/*                                 UPDATE                                     */
/******************************************************************************/
void GameLayer::updateCurrentBombs(float dt) noexcept {
  weapon_manager_->increaseOneCurrentWeaponsNumber(Package::Kind::Bomb);
  if (weapon_manager_->bombs()->currentWeaponsNumber() == 5) {
    this->unschedule(schedule_selector(GameLayer::updateCurrentBombs));
  }
}

void GameLayer::launchFirstAsteroidScheduled(float dt) noexcept {
  if (gc_->show_hints()) {
    asteroid_hint_->runAction(
          CC::Sequence::create(
            CC::DelayTime::create(3.f),
            CC::CallFunc::create([this](){
      CC_DIRECTOR_->getEventDispatcher()->resumeEventListenersForTarget(
         asteroid_hint_);
      this->pauseGame();
      asteroid_hint_->runAction(
            CC::Sequence::create(
              CC::FadeIn::create(1.5f),
              CC::DelayTime::create(3.f),
              CC::FadeOut::create(1.5f),
              CC::CallFunc::create([this](){
        this->resumeGame();
      }),
              nullptr));
    }),
            nullptr));
  }
  this->schedule(schedule_selector(GameLayer::updateAsteroidsLauncherInterval),
                 9.f);
  this->updateLaunchAsteroid(0.f);
  asteroids_laucher_interval_ = 7.5f;
  this->schedule(schedule_selector(GameLayer::updateLaunchAsteroid),
                 asteroids_laucher_interval_);
}

void GameLayer::launchFirstCloudScheduled(float dt) noexcept {
  if (gc_->show_hints()) {
    cloud_hint_->runAction(
          CC::Sequence::create(
            CC::DelayTime::create(60.f),
            CC::CallFunc::create([this](){
      CC_DIRECTOR_->getEventDispatcher()->resumeEventListenersForTarget(
         cloud_hint_);
      this->pauseGame();
      cloud_hint_->runAction(
            CC::Sequence::create(
              CC::FadeIn::create(1.5f),
              CC::DelayTime::create(3.f),
              CC::FadeOut::create(1.5f),
              CC::CallFunc::create([this](){
        this->resumeGame();
      }),
              nullptr));
    }),
            nullptr));
  }
  this->updateLaunchCloud(0.f);
  this->schedule(schedule_selector(GameLayer::updateCloudssLauncherInterval),
                 20.f);
  this->schedule(schedule_selector(GameLayer::updateLaunchCloud),
                 clouds_laucher_interval_);
}

void GameLayer::launchFirstPackageScheduled(float dt) noexcept {
  if (gc_->show_hints()) {
    packageid_hint_->runAction(
          CC::Sequence::create(
            CC::DelayTime::create(3.f),  // CORREGIR: ee
            CC::CallFunc::create([this](){
      CC_DIRECTOR_->getEventDispatcher()->resumeEventListenersForTarget(
         packageid_hint_);
      this->pauseGame();
      packageid_hint_->runAction(
            CC::Sequence::create(
              CC::FadeIn::create(1.5f),
              CC::DelayTime::create(3.f),
              CC::FadeOut::create(1.5f),
              CC::CallFunc::create([this](){
        this->resumeGame();
      }),
              nullptr));
    }),
            nullptr));
  }
  this->schedule(schedule_selector(GameLayer::updatePackagesLauncherInterval),
                 15.f);
  packages_->setPendantPackages(
        Package::Kind::Tornado,
        packages_->pendantPackages(Package::Kind::Tornado) + 1);
  packages_->launchNext(Package::Kind::Tornado, reverse_speed_);
  packages_laucher_interval_ = 28.f;
  this->schedule(schedule_selector(GameLayer::updateLaunchPackage),
                 packages_laucher_interval_);
}

void GameLayer::updateExtraWidthFactor(float dt) noexcept {
  gc_->set_extra_width_factor(gc_->extra_width_factor() + .07f);
  if (gc_->extra_width_factor() >= 0.78f) {
    this->unschedule(schedule_selector(GameLayer::updateExtraWidthFactor));
  }
}

void GameLayer::updateAsteroidsLauncherInterval(float dt) noexcept {
  if (asteroids_laucher_interval_ >= 2.f) {
    asteroids_laucher_interval_ -= .23f;
  } else {
    this->unschedule(
          schedule_selector(GameLayer::updateAsteroidsLauncherInterval));
  }
  this->schedule(schedule_selector(GameLayer::updateLaunchAsteroid),
                 asteroids_laucher_interval_);
}

void GameLayer::updateCloudssLauncherInterval(float dt) noexcept {
  if (clouds_laucher_interval_ >= 3.f) {  // CORREGIR: ee
    clouds_laucher_interval_ -= .6f;
  } else {
    this->unschedule(
          schedule_selector(GameLayer::updateCloudssLauncherInterval));
  }
  this->schedule(schedule_selector(GameLayer::updateLaunchCloud),
                 clouds_laucher_interval_);
}

void GameLayer::updatePackagesLauncherInterval(float dt) noexcept {
  if (packages_laucher_interval_ >= 4.f) {
    packages_laucher_interval_ -= .7f;
  } else {
    this->unschedule(
          schedule_selector(GameLayer::updatePackagesLauncherInterval));
  }
  this->schedule(schedule_selector(GameLayer::updateLaunchPackage),
                 packages_laucher_interval_);
}

void GameLayer::updateReverseSpeedInterval(float dt) noexcept {
  if (reverse_speed_ >= 2.f) {
    reverse_speed_ -= .25f;
  } else {
    this->unschedule(schedule_selector(GameLayer::updateReverseSpeedInterval));
  }
}

void GameLayer::updateLaunchPackage(float dt) noexcept {
  if (packages_->pendantPackages(Package::Kind::Healt) > 0 &&
      healt_->healt() < 100) {
    packages_->setPendantPackages(
          Package::Kind::Healt,
          packages_->pendantPackages(Package::Kind::Healt) + 1);
    packages_->launchNext(Package::Kind::Healt, reverse_speed_);
  }
  if (healt_->healt() <= 60) {
    packages_->setPendantPackages(
          Package::Kind::Healt,
          packages_->pendantPackages(Package::Kind::Healt) + 3);
    packages_->launchNext(Package::Kind::Healt, reverse_speed_);
    if (healt_->healt() <= 40) {
      packages_->setPendantPackages(
            Package::Kind::Healt,
            packages_->pendantPackages(Package::Kind::Healt) + 1);
      packages_->launchNext(Package::Kind::Healt, reverse_speed_);
    }
  }

  if (calendar_->passed_days() >= 1) {
    if (weapon_manager_->tornados()->currentWeaponsNumber() <= 2 ||
        packages_->pendantPackages(Package::Kind::Tornado)) {
      packages_->launchNext(Package::Kind::Tornado, reverse_speed_);
    }
  }

  if (calendar_->passed_days() >= 2 ||
      packages_->pendantPackages(Package::Kind::AtomicBomb) > 0) {
    packages_->launchNext(Package::Kind::AtomicBomb, reverse_speed_);
  }
}

void GameLayer::bombCheckCollitions(
    Bomb* bombWave,
    std::uint8_t *inOutNeutralizedMenaces,
    std::uint8_t *inOutHelpersBrokens) noexcept {
  *inOutNeutralizedMenaces = 0;
  *inOutHelpersBrokens = 0;
  float explosionRatio{bombWave->getBoundingBox().size.width*.5f};
  float waveX{bombWave->getPositionX()};
  float waveY{bombWave->getPositionY()};
  float objectRatio, objectX, objectY, objectDistance;
  for (Asteroid *meteor : asteroids_->meteors()) {
    if (meteor->isVisible() &&
        !meteor->getActionByTag(
           static_cast<int>(Asteroid::ActTag::explosion)) &&
        meteor->getActionByTag(static_cast<int>(Asteroid::ActTag::moveTo))) {
      objectRatio = meteor->getBoundingBox().size.width*.5f;
      objectX = meteor->getPositionX();
      objectY = meteor->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        ++exploded_meteors_;
        meteor->destroy(deceased_performed_callback_);
        this->increaseScore(15);
        ++(*inOutNeutralizedMenaces);
      }
    }
  }
  for (Package *package : packages_->packages()) {
    if (package->isVisible()) {
      objectRatio = package->getBoundingBox().size.width*.5f;
      objectX = package->getPositionX();
      objectY = package->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        ++(*inOutHelpersBrokens);
        package->stopAllActions();
        package->setVisible(false);
      }
    }
  }

  for (GenericWeapon* bomb : weapon_manager_->bombs()->weapons()) {
    this->chainReaction(bombWave, bomb);
  }
  for (GenericWeapon* bomb : weapon_manager_->atomicBombs()->weapons()) {
    this->chainReaction(bombWave, bomb);
  }
}

void GameLayer::atomicBombCheckCollitions(AtomicBomb *atomicBombWave) noexcept {
  float explosionRatio{atomicBombWave->getBoundingBox().size.width*.5f};
  float waveX{atomicBombWave->getPositionX()};
  float waveY{atomicBombWave->getPositionY()};
  float objectRatio, objectX, objectY, objectDistance;
  for (Asteroid *asteroid : asteroids_->meteors()) {
    if (asteroid->isVisible() &&
        !asteroid->getActionByTag(
           static_cast<int>(Asteroid::ActTag::explosion)) &&
        asteroid->getActionByTag(static_cast<int>(Asteroid::ActTag::moveTo))) {
      objectRatio = asteroid->getBoundingBox().size.width*.5f;
      objectX = asteroid->getPositionX();
      objectY = asteroid->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        ++exploded_meteors_;
        asteroid->destroy(deceased_performed_callback_);
        this->increaseScore(15);
      }
    }
  }
  for (Package *package : packages_->packages()) {
    if (package->isVisible()) {
      objectRatio = package->getBoundingBox().size.width*.5f;
      objectX = package->getPositionX();
      objectY = package->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        package->stopAllActions();
        package->setVisible(false);
      }
    }
  }

  for (GenericWeapon* bomb : weapon_manager_->atomicBombs()->weapons()) {
    this->chainReaction(atomicBombWave, bomb);
  }
  for (GenericWeapon* bomb : weapon_manager_->bombs()->weapons()) {
    this->chainReaction(atomicBombWave, bomb);
  }
}

void GameLayer::tornadoCheckCollitions(
    Tornado *tornado,
    std::uint8_t *inOutNeutralizedMenaces,
    std::uint8_t *inOutHelpersBrokens) noexcept {
  *inOutNeutralizedMenaces = 0;
  *inOutHelpersBrokens = 0;
  float explosionRatio{tornado->getBoundingBox().size.height*.5f};
  float waveX{tornado->getPositionX()};
  float waveY{tornado->getPositionY()};
  float objectRatio, objectX, objectY, objectDistance;
  for (Cloud *cloud : clouds_->clouds()) {
    if (cloud->isVisible() &&
        !cloud->getActionByTag(static_cast<int>(Cloud::ActionTag::puffed))) {
      objectRatio = cloud->getBoundingBox().size.width*.5f;
      objectX = cloud->getPositionX();
      objectY = cloud->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        cloud->puff(tornado->getPosition());
        ++(*inOutNeutralizedMenaces);
      }
    }
  }
  for (Package *package : packages_->packages()) {
    if (package->isVisible() &&
        !package->getActionByTag(
           static_cast<int>(Package::ActionTag::puffed))) {
      objectRatio = package->getBoundingBox().size.width*.5f;
      objectX = package->getPositionX();
      objectY = package->getPositionY();
      objectDistance = sqrt(pow((waveX - objectX), 2) +
                       pow((waveY - objectY), 2));
      if (objectDistance <= explosionRatio + objectRatio) {
        package->puff(tornado->getPosition());
        ++(*inOutHelpersBrokens);
      }
    }
  }
}

void GameLayer::chainReaction(GenericWeapon *wave,
                              GenericWeapon *weapon) noexcept {
  float explosionRatio{wave->getBoundingBox().size.width*.5f};
  float waveX{wave->getPositionX()};
  float waveY{wave->getPositionY()};
  float objectRatio, objectX, objectY, objectDistance;
  if (weapon->isVisible() &&
      weapon->getOpacity() >= 255 &&
      !weapon->getActionByTag(static_cast<int>(GenericWeapon::Tag::explodin))) {
    objectRatio = weapon->getBoundingBox().size.width*.5f;
    objectX = weapon->getPositionX();
    objectY = weapon->getPositionY();
    objectDistance = sqrt(pow((waveX - objectX), 2) +
                     pow((waveY - objectY), 2));
    if (objectDistance <= explosionRatio + objectRatio) {
      // FIXME dynamic cast
      if (dynamic_cast<Bomb*>(weapon) != nullptr) {
        weapon_manager_->explodeWeapon(static_cast<Bomb*>(weapon));
      } else if (dynamic_cast<AtomicBomb*>(weapon) != nullptr) {
        weapon_manager_->explodeWeapon(static_cast<AtomicBomb*>(weapon));
      }
    }
  }
}

void GameLayer::updateBombsCollitions(float dt) noexcept {
  // WARNING(denisacostaq@gmail.com): si el tamanno del vector cambia
  // hay que actualizar el valor de weapons que solo se pone en la
  // inicializacion.
  static CC::Vector<GenericWeapon*> weapons{
    weapon_manager_->bombs()->weapons()};
  for (GenericWeapon *bomb : weapons) {
    if (bomb->is_exploding()) {
      std::uint8_t inOutNeutralizedAsteroid{0};
      std::uint8_t inOutPackagesBrokens{0};
      this->bombCheckCollitions(static_cast<Bomb*>(bomb),
                                &inOutNeutralizedAsteroid,
                                &inOutPackagesBrokens);
      bomb->set_neutralized_menaces_by_last_shoot(
         bomb->neutralized_menaces_by_last_shoot() +
           inOutNeutralizedAsteroid);
      bomb->set_helpers_brokens_by_shoot(
         bomb->helpers_brokens_by_shoot() +
           inOutPackagesBrokens);

    }
  }
}

void GameLayer::updateAtomicBomsCollitions(float dt) noexcept {
  // WARNING(denisacostaq@gmail.com): si el tamanno del vector cambia
  // hay que actualizar el valor de weapons que solo se pone en la
  // inicializacion.
  static CC::Vector<GenericWeapon*> weapons{
    weapon_manager_->atomicBombs()->weapons()};
  for (GenericWeapon *aBomb : weapons) {
    if (aBomb->is_exploding()) {
      this->atomicBombCheckCollitions(static_cast<AtomicBomb*>(aBomb));
    }
  }
}

void GameLayer::updateTornadosCollitions(float dt) noexcept {
  // WARNING(denisacostaq@gmail.com): si el tamanno del vector cambia
  // hay que actualizar el valor de weapons que solo se pone en la
  // inicializacion.
  static CC::Vector<GenericWeapon*> weapons{
    weapon_manager_->tornados()->weapons()};
  for (GenericWeapon *tornado : weapons) {
    if (tornado->is_exploding()) {
      std::uint8_t inOutNeutralizedMenaces{0};
      std::uint8_t inOutHelpersBrokens{0};
      this->tornadoCheckCollitions(static_cast<Tornado*>(tornado),
                                   &inOutNeutralizedMenaces,
                                   &inOutHelpersBrokens);
      tornado->set_neutralized_menaces_by_last_shoot(
         tornado->neutralized_menaces_by_last_shoot() +
           inOutNeutralizedMenaces);
      tornado->set_helpers_brokens_by_shoot(
            tornado->helpers_brokens_by_shoot() + inOutHelpersBrokens);
    }
  }
}

void GameLayer::deceasedPerformedCallback(Node* pSender) noexcept {
  switch (static_cast<NodeTag>(pSender->getTag())) {
    case NodeTag::meteor:
      this->asteroidDone(static_cast<Asteroid*>(pSender));
      break;
    case NodeTag::packet:
      this->packageFalled(static_cast<Package*>(pSender));
      break;
    default:
      break;
  }
}

void GameLayer::packageFalled(Package *package) noexcept {
  switch (package->getKind()) {
    case Package::Kind::Healt:
      if (healt_->healt() < 100) {
        gc_->playEffect("package_taken.ogg");
        healt_->set_healt(healt_->healt() + 10);
      }
      break;
    case Package::Kind::AtomicBomb:
    case Package::Kind::Tornado:
    case Package::Kind::Bomb:
      weapon_manager_->increaseOneCurrentWeaponsNumber(package->getKind());
      break;
    default:
      break;
  }

  package->setToInitialState();
}

void GameLayer::asteroidFalled(Asteroid *asteroid) noexcept {
  asteroid->impact();
  if (healt_->healt() - 10 < 0) {
    healt_->set_healt(0);
  } else {
    healt_->set_healt(healt_->healt() - 10);
  }
  if (healt_->healt() == 0) {
    this->pauseGame();
    this->stopGame();
    menu_->setEnabled(false);
    char str[20]{0};
    std::snprintf(str, sizeof(str), "%d", calendar_->passed_days());
    days_go_label_->setString(std::string{str});
    std::memset(str, 0, sizeof(str));
    // std::snprintf(str, sizeof(str), "%d", score_);
    score_go_label_->setString(std::to_string(score_));
    std::memset(str, 0, sizeof(str));
    float accurancy{.0f};
    if (used_bombs_ > 0) {
      accurancy = static_cast<float>(brocken_asteroids_with_boms_)/used_bombs_;
    }
    std::snprintf(str,
                sizeof(str),
                "%u",
                static_cast<std::uint16_t>(accurancy * 100));
    accurancy_label_->setString(std::string{str});
    std::uint16_t score{
      static_cast<std::uint16_t>(
            (calendar_->passed_days() + 1) * score_ * accurancy)};
    gc_->set_height_score(score);
    std::memset(str, 0, sizeof(str));
    std::snprintf(str, sizeof(str), "%u", score);
    total_score_label_->setString(std::string{str});
    std::memset(str, 0, sizeof(str));
    std::snprintf(str, sizeof(str), "%u", gc_->height_score());
    heigth_score_->setString(std::string{str});

    game_over_sprite_->setVisible(true);
    gameover_info_->setScale(.1f);
    gameover_info_->setVisible(true);
    gameover_info_->runAction(CC::ScaleTo::create(1.3f, 1.f));
    healt_->setVisible(false);
    healt_->healt_background()->setVisible(false);
    weapon_manager_->setVisible(false);
    menu_->setVisible(false);
    score_label_->setVisible(false);
    calendar_->setVisible(false);
//  } else if (_healt->healt() < 20) {
//    // gameOverSprite_->setOpacity(120);
//    _gc->playEffect("Level1/fire_truck.wav");
//  } else if (_healt->healt() < 30) {
//    // gameOverSprite_->setOpacity(70);
//    _gc->playEffect("Level1/fire_truck.wav");
  }
//  else if (_healt->healt() < 50) {
//    static std::chrono::steady_clock::time_point lastPlayed{
//      std::chrono::steady_clock::now()};
//    std::chrono::steady_clock::time_point now{
//      std::chrono::steady_clock::now()};
//    CC::log("\n\nPrinting took %ld\n\n",
//            std::chrono::duration_cast<std::chrono::milliseconds>(
//              now - lastPlayed).count());
//    if (std::chrono::duration_cast<std::chrono::milliseconds>(
//          now - lastPlayed).count()/1000 > 12) {
//      lastPlayed = std::chrono::steady_clock::now();
//      _gc->playEffect("Level1/fire_truck.wav");
//    }
//  }
}

void GameLayer::weaponExplodedCallback(CC::Node *pSender) noexcept {
  std::uint8_t neutralizedMenaces{0};
  std::uint8_t helpersBrokens{0};
  switch (static_cast<GenericWeapon::Kind>(pSender->getTag())) {
    case GenericWeapon::Kind::Bomb:
      neutralizedMenaces =
          static_cast<Bomb*>(pSender)->neutralized_menaces_by_last_shoot();
      helpersBrokens =
          static_cast<Bomb*>(pSender)->helpers_brokens_by_shoot();
      static_cast<Bomb*>(pSender)->loot(
            increaseScoreFunction,
            calendar_->passed_days_background()->getPosition() +
              CC::Vec2{score_label_->getPositionX(), .0f});
      if (neutralizedMenaces >= 3 && helpersBrokens == 0) {
        packages_->setPendantPackages(
              Package::Kind::AtomicBomb,
              packages_->pendantPackages(Package::Kind::AtomicBomb) + 2);
      } else if (neutralizedMenaces == 2 &&
                   ++count_rltzt_acum_ == count_rltzt_) {
        this->asteroidsRelentization();
        count_rltzt_acum_ = 0;
      }
      if (helpersBrokens >= 2 && clouds_->visibleClouds() >= 3) {
        packages_->setPendantPackages(
              Package::Kind::Tornado,
              packages_->pendantPackages(Package::Kind::Tornado) + 2);
      }
      brocken_asteroids_with_boms_ += neutralizedMenaces;
      ++used_bombs_;
      break;
    case GenericWeapon::Kind::AtomicBomb:
      // CC::log("AtomicBomb explodedddddddddddddddd");
      break;
    case GenericWeapon::Kind::Tornado:
      neutralizedMenaces =
          static_cast<Bomb*>(pSender)->neutralized_menaces_by_last_shoot();
      helpersBrokens =
          static_cast<Bomb*>(pSender)->helpers_brokens_by_shoot();
//      if (neutralizedMenaces <= 3 &&
//          helpersBrokens == 0 &&
//          _clouds->visibleClouds() >= 2) {
//        CC::log("LANZAR2:                tornadossssssss");
//      }
      break;
    default:
      break;
  }
}

void GameLayer::asteroidsRelentization() noexcept {
  for (Asteroid *meteor : asteroids_->meteors()) {
    if (meteor->getActionByTag(static_cast<int>(Asteroid::ActTag::moveTo))) {
      meteor->stopActionByTag(static_cast<int>(Asteroid::ActTag::moveTo));
      float time{(3 * reverse_speed_ * meteor->getPositionY()) /
          visible_size_.height};
      CC::Sequence *seq{CC::Sequence::create(
          CC::MoveTo::create(time, meteor->destiny()),
          CC::CallFuncN::create(
            CC_CALLBACK_1(GameLayer::deceasedPerformedCallback, this)),
          nullptr)};
      seq->setTag(static_cast<int>(Asteroid::ActTag::moveTo));
      meteor->runAction(seq);
    }
  }
  CC::log("la funcioncitaaa");
}


/******************************************************************************/
/*                                TOUCHES                                     */
/******************************************************************************/
bool GameLayer::onTouchBegan(CC::Touch *touch, CC::Event *event) {
  if (gc_->paused()) {
    return false;
  }
  are_touching_ = true;
  // if false no onTouchMoved, no onTouchEnd
  return menu_->isEnabled() &&
          !paralax_root_node_->getActionByTag(
            static_cast<int>(NodeTag::paralax));
}

void GameLayer::onTouchMoved(CC::Touch *touch, CC::Event *event) {
  if (paralax_is_active_) {
    if (std::abs(touch->getDelta().x) > visible_size_.width*.01f) {
      are_touching_ = false;
      paralax_hint_->getChildren().at(0)->stopAllActions();
      paralax_hint_->getChildren().at(1)->stopAllActions();
      paralax_hint_->setVisible(false);
      if (touch->getDelta().x < 0) {
        if (paralax_root_node_->getPositionX() + touch->getDelta().x >=
            -visible_size_.width * (gc_->extra_width_factor()*.5f) +
              visible_origin_.x) {
          paralax_root_node_->setPositionX(
                paralax_root_node_->getPositionX() + touch->getDelta().x);
        } else {
          paralax_root_node_->setPositionX(
                -visible_size_.width * (gc_->extra_width_factor()*.5f) +
                  visible_origin_.x);
        }
      } else {
        if (paralax_root_node_->getPositionX() + touch->getDelta().x <=
            visible_size_.width * gc_->extra_width_factor()*.5f +
              visible_origin_.x) {
          paralax_root_node_->setPositionX(
                paralax_root_node_->getPositionX() + touch->getDelta().x);
        } else {
          paralax_root_node_->setPositionX(
                visible_size_.width * gc_->extra_width_factor()*.5f +
                  visible_origin_.x);
        }
      }
    }
  }
}

void GameLayer::onTouchEnded(CC::Touch *touch, CC::Event *event) {
  if (are_touching_) {
    weapon_manager_->putWeaponIn(
          CC::Vec2{
            touch->getLocation().x - paralax_root_node_->getPositionX()*.87f,
            touch->getLocation().y - visible_origin_.y});
    // BUG
  }
}


/******************************************************************************/
/*                               CALLBACKS                                    */
/******************************************************************************/
void GameLayer::menuPauseCallback() noexcept {
  CC::log("void GameLayer::menuPauseCallback() {");
  if (paralax_root_node_->getActionByTag(static_cast<int>(NodeTag::paralax))) {
    CC_DIRECTOR_->getActionManager()->pauseTarget(paralax_root_node_);
  } else {
    hints_actions_locket_ = false;
    pauseGame();
  }
  menu_->setVisible(false);
  menu_on_pause_->setVisible(true);
  menu_items_background_->setVisible(true);
}

void GameLayer::menuResumeCallback() noexcept {
  if (paralax_root_node_->getActionByTag(static_cast<int>(NodeTag::paralax))) {
    CC_DIRECTOR_->getActionManager()->resumeTarget(paralax_root_node_);
  } else {
    resumeGame();
    hints_actions_locket_ = true;
  }
  menu_on_pause_->setVisible(false);
  menu_items_background_->setVisible(false);
  menu_->setVisible(true);
}

void GameLayer::menuRestartCallback() noexcept {
  menuResumeCallback();
  stopGame();
  setToInitialState();
  if (gc_->audio_enabled()) {
    CC_AUDIO_->pauseBackgroundMusic();
    CC_AUDIO_->stopAllEffects();
    this->runAction(CC::Sequence::createWithTwoActions(
                      CC::DelayTime::create(1.5f),
                      CC::CallFunc::create([this]{
      CC_AUDIO_->resumeBackgroundMusic();
      CC_AUDIO_->rewindBackgroundMusic();
    })));
  }
}

void GameLayer::menuExitCallback() noexcept {
  menuResumeCallback();
  stopGame();
  CC_DIRECTOR_->popScene();
}

#ifdef EVALUATION_VERSION
  void GameLayer::menuDownloadCallback() noexcept {
    CC::log("open URL");
    // GlobalColaborationNP::openURL("url");
  }
#endif

void GameLayer::pauseGame() noexcept {
  gc_->pause();
  // BUG(denisacostaq@gmail.com): si esta pausado por los hints y lo
  // pauso/resumo se resume por encima de lo k "diga" hints.
  if (!hints_actions_locket_) {
    if (gc_->show_hints() == false) {
      CC_DIRECTOR_->getActionManager()->pauseTarget(
            paralax_hint_->getChildren().at(0));
      CC_DIRECTOR_->getActionManager()->pauseTarget(
            paralax_hint_->getChildren().at(1));
    }
    CC_DIRECTOR_->getActionManager()->pauseTarget(asteroid_hint_);
    CC_DIRECTOR_->getActionManager()->pauseTarget(packageid_hint_);
    CC_DIRECTOR_->getActionManager()->pauseTarget(cloud_hint_);
  }
  weapon_manager_->pause();
  clouds_->pause();
  calendar_->pause();
  packages_->pause();
  asteroids_->pause();
  healt_->pause();
  this->pause();
  if (gc_->audio_enabled()) {
    CC_AUDIO_->pauseBackgroundMusic();
    CC_AUDIO_->pauseAllEffects();
  }
  AdmobHelper::showAd();
}

void GameLayer::resumeGame() noexcept {
  CC_AUDIO_->resumeAllEffects();
  CC_AUDIO_->resumeBackgroundMusic();
  this->resume();
  healt_->resume();
  asteroids_->resume();
  packages_->resume();
  clouds_->resume();
  calendar_->resume();
  weapon_manager_->resume();
  if (!hints_actions_locket_) {
    if (gc_->show_hints() == false) {
      CC_DIRECTOR_->getActionManager()->resumeTarget(
            paralax_hint_->getChildren().at(0));
      CC_DIRECTOR_->getActionManager()->resumeTarget(
            paralax_hint_->getChildren().at(1));
    }
    CC_DIRECTOR_->getActionManager()->resumeTarget(asteroid_hint_);
    CC_DIRECTOR_->getActionManager()->resumeTarget(packageid_hint_);
    CC_DIRECTOR_->getActionManager()->resumeTarget(cloud_hint_);
  }
  gc_->resume();
  AdmobHelper::hideAd();
}

void GameLayer::stopGame() noexcept {
  healt_->stop();
  asteroids_->stop();
  weapon_manager_->stop();
  calendar_->stop();
  clouds_->stop();
  packages_->stop();
  paralax_root_node_->stopAllActions();
  asteroid_hint_->stopAllActions();
  cloud_hint_->stopAllActions();
  paralax_hint_->getChildren().at(0)->stopAllActions();
  paralax_hint_->getChildren().at(1)->stopAllActions();
  packageid_hint_->stopAllActions();

  // SpriteFrameCache::getInstance()->removeSpriteFrames();
  // CC::SpriteFrameCache::getInstance()->destroyInstance();
  // FIXME: if use this, cause a memory leak?

  this->unschedule(schedule_selector(GameLayer::updateAtomicBomsCollitions));
  this->unschedule(schedule_selector(GameLayer::updateBombsCollitions));
  this->unschedule(schedule_selector(GameLayer::updateTornadosCollitions));
  this->unschedule(schedule_selector(GameLayer::updateCurrentBombs));
  this->unschedule(schedule_selector(GameLayer::launchFirstAsteroidScheduled));
  this->unschedule(schedule_selector(GameLayer::launchFirstCloudScheduled));
  this->unschedule(schedule_selector(GameLayer::launchFirstPackageScheduled));
  this->unschedule(schedule_selector(GameLayer::updateReverseSpeedInterval));
  this->unschedule(schedule_selector(GameLayer::automaticParalaxEfect));
#ifdef EVALUATION_VERSION
  this->unschedule(schedule_selector(GameLayer::evaluationVersionScheduled));
#endif
  this->unschedule(schedule_selector(GameLayer::onParalaxActivated));
  this->unschedule(
     schedule_selector(GameLayer::updateAsteroidsLauncherInterval));
  this->unschedule(schedule_selector(GameLayer::updateLaunchAsteroid));
  this->unschedule(schedule_selector(GameLayer::updateCloudssLauncherInterval));
  this->unschedule(schedule_selector(GameLayer::updateLaunchCloud));
  this->unschedule(
     schedule_selector(GameLayer::updatePackagesLauncherInterval));
  this->unschedule(schedule_selector(GameLayer::updateLaunchPackage));
  this->unschedule(schedule_selector(GameLayer::updateExtraWidthFactor));

  // stopSound();
}

void GameLayer::deleteSound() noexcept {
  if (gc_->audio_enabled()) {
    CC_AUDIO_->stopBackgroundMusic(true);

    CC_AUDIO_->unloadEffect("alarm.ogg");
    CC_AUDIO_->unloadEffect("asteroid.ogg");
    CC_AUDIO_->unloadEffect("bomb.ogg");
    CC_AUDIO_->unloadEffect("medall.ogg");
    CC_AUDIO_->unloadEffect("package_taken.ogg");
    CC_AUDIO_->unloadEffect("MainMenu/show_credits.ogg");

    CC_AUDIO_->stopAllEffects();
  }
}
