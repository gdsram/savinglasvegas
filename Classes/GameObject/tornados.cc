/*! \brief This file have the implementation for Tornados class
    \file tornados.cc
    \author Copyright owners
    \date 6/25/2014

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

#include "Classes/GameObject/tornados.h"

Tornados::Tornados(CC::Layer *gameLayer, CC::SEL_SCHEDULE collitioner_checker)
  : GenericWeapons{5, gameLayer, collitioner_checker}
  , visible_origin_{CC::Vec2::ZERO} {
}

Tornados* Tornados::create(
    CC::Layer *gameLayer,
    CC::SEL_SCHEDULE collitioner_checker,
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept {
  Tornados *pRet{new Tornados{gameLayer, collitioner_checker}};
  if (pRet && pRet->init(weaponExplodedCallback)) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool Tornados::init(
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept {
  GenericWeapons::init();
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  for (int i = 0; i < cMaxBombsNumber_; ++i) {
    Tornado *tornado{Tornado::create(weaponExplodedCallback)};
    this->weapons_.pushBack(tornado);
  }
  CC::Animation* expansiveWave{CC::Animation::create()};
  expansiveWave->retain();
  char path[200];
  memset(path, 0, sizeof(path));
  for (std::uint8_t i{1}; i <= 8; ++i) {
    std::snprintf(
          path,
          sizeof(path),
          "Animations/tornado_puffing/tornado_puffing_%u.png",
          i);
    expansiveWave->addSpriteFrame(CC::Sprite::create(path)->getSpriteFrame());
  }
  expansiveWave->setDelayPerUnit(.5f/4.f);
  expansiveWave->setRestoreOriginalFrame(false);
  CC::AnimationCache::getInstance()->addAnimation(
        expansiveWave,
        "TornadoAnimation");
  CC_SAFE_RELEASE(expansiveWave);
  return true;
}

void Tornados::setToInitialState() noexcept {
  for (GenericWeapon *tornado : weapons_) {
     static_cast<Tornado*>(tornado)->setToInitialState();
  }
  current_weapons_number_ = 0;
  GenericWeapons::setToInitialState();
}

void Tornados::putWeaponIn(const CC::Vec2 position) noexcept {
  for (std::uint8_t i{0}, inUseWeapons{0};
       inUseWeapons < current_weapons_number_;
       ++i) {
    if (weapons_.at(i)->isVisible()) {
      ++inUseWeapons;
    } else {
      weapons_.at(i)->putIn(position);
      this->explodeWeapon(weapons_.at(i));
      break;
    }
  }
}
