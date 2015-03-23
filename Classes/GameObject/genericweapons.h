/*! \brief This file have the interface for GenericBombs class
    \file genericweapons.h
    \author Copyright owners
    \date 6/22/2014

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

#ifndef CLASSES_GAMEOBJECT_GENERICBOMBS_H
#define CLASSES_GAMEOBJECT_GENERICBOMBS_H

#include <cstdint>
#include <functional>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/appmacros.h"
#include "Classes/GameObject/genericweapon.h"

class GenericWeapons : public CC::Ref {
 public:
  GenericWeapons(const GenericWeapons&) = delete;
  GenericWeapons& operator=(const GenericWeapons&) = delete;

  GenericWeapons(const GenericWeapons&&) = delete;
  GenericWeapons& operator=(const GenericWeapons&&) = delete;

  static GenericWeapons* create() = delete;
  inline void init() {
    visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  }

  inline void explodeWeapon(const GenericWeapon *weapon) noexcept {
    if (++exploding_ == 1) {
      game_layer_->schedule(coll_chk_);
    }
    const_cast<GenericWeapon*>(weapon)->explode(
          CC::CallFuncN::create(CC_CALLBACK_1(GenericWeapons::exploded,
                                              this)));
  }
  virtual inline void exploded(CC::Node* pSender) noexcept {
    if (--exploding_ == 0) {
      game_layer_->unschedule(coll_chk_);
    }
    static_cast<GenericWeapon*>(pSender)->exploded();
    int weaponTag{pSender->getTag()};
    if (weaponTag == static_cast<int>(GenericWeapon::Kind::Tornado) ||
        weaponTag == static_cast<int>(GenericWeapon::Kind::AtomicBomb)) {
      decreaseOneCurrentWeaponsNumber();
    }
  }
  inline const CC::Vector<GenericWeapon*>& weapons() const noexcept {
    return weapons_;
  }
  inline const std::uint8_t currentWeaponsNumber() const noexcept {
    return current_weapons_number_;
  }
  inline void decreaseOneCurrentWeaponsNumber() noexcept {
    if (current_weapons_number_ > 0) {
      --current_weapons_number_;
    }
  }
  // TODO: Poner privada y hacer frind de WeaponHandler
  void increaseOneCurrentWeaponsNumber() noexcept;
  virtual void putWeaponIn(const CC::Point position) noexcept = 0;
  virtual inline void pause() noexcept {
    for (GenericWeapon *bomb : weapons_) {
      bomb->pause();
    }
    if (game_layer_->isScheduled(coll_chk_)) {
      game_layer_->unschedule(coll_chk_);
    }
  }
  virtual inline void resume() noexcept {
    if (exploding_ > 0) {
      game_layer_->schedule(coll_chk_);
    }
    for (GenericWeapon *bomb : weapons_) {
      bomb->resume();
    }
  }
  virtual inline void stop() noexcept {
    for (GenericWeapon *bomb : weapons_) {
      bomb->stop();
    }
    if (game_layer_->isScheduled(coll_chk_)) {
      game_layer_->unschedule(coll_chk_);
    }
  }
  virtual void setToInitialState() noexcept {
    if (game_layer_->isScheduled(coll_chk_)) {
      game_layer_->unschedule(coll_chk_);
    }
    exploding_ = 0;
  }

 protected:
  const std::uint8_t cMaxBombsNumber_;
  CC::SEL_SCHEDULE coll_chk_;
  std::uint8_t current_weapons_number_;
  CC::Vector<GenericWeapon*> weapons_;
  std::uint8_t exploding_;
  CC::Layer *game_layer_;
  CC::Vec2 visible_origin_;

  GenericWeapons(std::uint8_t max_bomb_number,
                 CC::Layer *gameLayer,
                 CC::SEL_SCHEDULE collitionerChecker);
  ~GenericWeapons() = default;

  // TODO(denisacostaq@gmail.com): esto va pa el arma especifica
  // y this->showBombDistchangers(); tiene que ser un callback.
  void initListener(GenericWeapon *genericWeapon) noexcept;
};

#endif  // CLASSES_GAMEOBJECT_GENERICBOMBS_H
