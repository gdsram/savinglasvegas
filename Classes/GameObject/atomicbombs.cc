/*! \brief This file have the implementation for AtomicBombs class
    \file atomicbombs.cc
    \author Copyright owners
    \date 6/21/2014

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

#include "Classes/GameObject/atomicbombs.h"

AtomicBombs::AtomicBombs(CC::Layer *gameLayer,
                         CC::SEL_SCHEDULE collitionerChecker)
  : GenericWeapons{5, gameLayer, collitionerChecker}
  , explode_weapon_{std::bind(&GenericWeapons::explodeWeapon,
                                   this,
                                   std::placeholders::_1)}
  , lighting_read_alarm_{nullptr} {
}

AtomicBombs* AtomicBombs::create(
    CC::Layer *gameLayer,
    CC::SEL_SCHEDULE collitioner_checker,
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept {
  AtomicBombs *pRet{new AtomicBombs{gameLayer, collitioner_checker}};
  if (pRet && pRet->init(weaponExplodedCallback)) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool AtomicBombs::init(
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept {
  GenericWeapons::init();
  CC::Size visibleSize{CC_DIRECTOR_->getVisibleSize()};
  lighting_read_alarm_ = CC::LayerColor::create(
        CC::Color4B{255, 0, 0, 30}, visibleSize.width, visibleSize.height);
  lighting_read_alarm_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE);
  lighting_read_alarm_->setPosition(
        CC::Vec2 {
          visibleSize.width*.5f -
            lighting_read_alarm_->getContentSize().width*.5f,
          visibleSize.height*.5f -
            lighting_read_alarm_->getContentSize().height*.5f} +
          visible_origin_);

  for (int i = 0; i < cMaxBombsNumber_; ++i) {
    AtomicBomb *bomb{
      AtomicBomb::create(
            weaponExplodedCallback,
            std::bind(&AtomicBombs::atomicBombDischargedCallback, this))};
    this->initListener(bomb);
    this->weapons_.pushBack(bomb);
    bomb->setVisible(false);
  }
  CC::Animation* expansiveWave{CC::Animation::create()};
  expansiveWave->retain();
  expansiveWave->addSpriteFrame(
        CC::Sprite::create("atomic_bomb_on.png")->getSpriteFrame());
  char path[150];
  for (std::uint8_t i{1}; i <= 6; ++i) {
    std::snprintf(
          path,
          sizeof(path),
          "Animations/atomic_bomb_explosion/atomic_bomb_explosion_%u.png",
          i);
    expansiveWave->addSpriteFrame(CC::Sprite::create(path)->getSpriteFrame());
  }
  expansiveWave->setDelayPerUnit(1.f / 7.f);
  expansiveWave->setRestoreOriginalFrame(false);
  CC::AnimationCache::getInstance()->addAnimation(
        expansiveWave,
        "AtomicBombExplotionAnimation");
  CC_SAFE_RELEASE(expansiveWave);
  return true;
}

void AtomicBombs::putWeaponIn(const CC::Vec2 position) noexcept {
  for (std::uint8_t i{0}, inUseWeapons{0};
       inUseWeapons < current_weapons_number_;
       ++i) {
    if (weapons_.at(i)->isVisible()) {
      ++inUseWeapons;
    } else {
      static_cast<AtomicBomb*>(weapons_.at(i))->putIn(
            position,
            explode_weapon_);
      if (lighting_read_alarm_->getActionByTag(34) == nullptr) {
        CC::Action *action{
          CC::RepeatForever::create(
                CC::Sequence::create(
                  CC::CallFunc::create([](){
                    G_COLABORATION_->playEffect("alarm.ogg");}),
                  CC::FadeTo::create(.8f, 200),
                  CC::FadeTo::create(.8f, 0),
                  CC::DelayTime::create(.5f),
                  nullptr))};
        action->setTag(34);
        lighting_read_alarm_->setVisible(true);
        lighting_read_alarm_->runAction(action);
      }
      break;
    }
  }
}

void AtomicBombs::atomicBombDischargedCallback() noexcept {
  bool zeroPreparing{true};
  for (GenericWeapon* weapon : weapons_) {
    if (weapon->isVisible() && !weapon->is_exploding()) {
      zeroPreparing = false;
      break;
    }
  }
  if (zeroPreparing) {
    lighting_read_alarm_->runAction(
          CC::Sequence::create(
            CC::FadeTo::create(.4, 0),
            CC::CallFunc::create([&](){
              lighting_read_alarm_->setVisible(false);
              // 4 _gc->playEffect("alarm.ogg");
              // FIXME(denisacostaq): esto es el hash.
              CC_AUDIO_->pauseEffect(4);
              lighting_read_alarm_->stopAllActions();}),
            nullptr));
  }
}
