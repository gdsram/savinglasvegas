/*! \brief This file have the interface for GenerciWeapon class.
    \file genericweapon.h
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

#ifndef CLASSES_GAMEOBJECT_GENERICWEAPON_H
#define CLASSES_GAMEOBJECT_GENERICWEAPON_H

#include <functional>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/globalcolaboration.h"

class GenericWeapon : public CC::Sprite {
 public:
  enum class Kind{Bomb, AtomicBomb, Tornado};
  enum class Tag {explodin};
  ~GenericWeapon() = default;

  GenericWeapon(const GenericWeapon&) = delete;
  GenericWeapon& operator=(const GenericWeapon&) = delete;

  GenericWeapon(const GenericWeapon&&) = delete;
  GenericWeapon& operator=(const GenericWeapon&&) = delete;

  static GenericWeapon* create() = delete;

  inline virtual bool initWithFile(
      const std::string& spriteFrameName) override {
    if (!Sprite::initWithFile(spriteFrameName)) {
      return false;
    }
    visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
    return true;
  }

  virtual void explode(CC::CallFuncN *onExploded) noexcept = 0;
  virtual void explode() noexcept {
    this->stop();
  }
  void set_is_exploding(bool exp) noexcept {
    is_exploding_ = exp;
  }
  bool is_exploding() const noexcept {
    return is_exploding_;
  }
  void exploded() noexcept {
    this->set_is_exploding(false);
    this->setVisible(false);
    weapon_exploded_callback_(this);
  }
  virtual inline void pause() noexcept {
    CC_DIRECTOR_->getActionManager()->pauseTarget(this);
  }
  virtual inline void resume() noexcept {
    CC_DIRECTOR_->getActionManager()->resumeTarget(this);
  }
  virtual inline void stop() noexcept {
    this->stopAllActions();
  }

  void activeBomb() noexcept {
    if (static_cast<Kind>(this->getTag()) == Kind::Bomb) {
      this->setSpriteFrame(CC::Sprite::create("bomb_on.png")->getSpriteFrame());
    } else if (static_cast<Kind>(this->getTag()) == Kind::AtomicBomb) {
      this->setSpriteFrame(
        CC::Sprite::create("atomic_bomb_on.png")->getSpriteFrame());
    }
    this->setOpacity(255);
  }
  inline void set_neutralized_menaces_by_last_shoot(
      std::uint8_t count) noexcept {  // TODO: increase
    neutralized_menaces_by_last_shoot_ = count;
  }
  inline void set_helpers_brokens_by_shoot(std::uint8_t count) noexcept {
    helpers_brokens_by_shoot_ = count;
  }
  inline const std::uint8_t neutralized_menaces_by_last_shoot() const noexcept {
    return neutralized_menaces_by_last_shoot_;
  }
  inline const std::uint8_t helpers_brokens_by_shoot() const noexcept {
    return helpers_brokens_by_shoot_;
  }
  virtual inline void setToInitialState() noexcept {
    neutralized_menaces_by_last_shoot_ = 0;
    this->set_helpers_brokens_by_shoot(0);
    this->setOpacity(0);
    this->setVisible(false);
    is_exploding_ = false;
  }
  virtual inline void putIn(const cocos2d::Vec2 &position) noexcept {
    this->setToInitialState();
    this->setScale(.1f);
    this->setOpacity(150);
    this->setPosition(position);
    this->setVisible(true);
  }

 protected:
  bool is_exploding_;

  // TODO(denisacostaq@gmail.com): mover esto a la clase bomb solamnete
  std::uint8_t neutralized_menaces_by_last_shoot_;
  std::uint8_t helpers_brokens_by_shoot_;
  CC::Point visible_origin_;

  const std::function<void(cocos2d::Node *)>& weapon_exploded_callback_;

  explicit GenericWeapon(
      const std::function<void(CC::Node *)> &weaponExplodedCallback);

 private:

};

#endif  // CLASSES_GAMEOBJECT_GENERICWEAPON_H
