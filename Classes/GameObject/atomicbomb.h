/*! \brief This file have the interface for AtomicBomb class
    \file atomicbomb.h
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

#ifndef CLASSES_GAMEOBJECT_ATOMICBOMB_H
#define CLASSES_GAMEOBJECT_ATOMICBOMB_H

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/GameObject/genericweapon.h"

class AtomicBomb : public GenericWeapon {
 public:
  ~AtomicBomb() = default;

  AtomicBomb(const AtomicBomb&) = delete;
  AtomicBomb& operator=(const AtomicBomb&) = delete;
  AtomicBomb(AtomicBomb&&) = delete;
  AtomicBomb& operator=(AtomicBomb&&) = delete;

  inline virtual void pause() noexcept override {
    GenericWeapon::pause();
    CC_DIRECTOR_->getEventDispatcher()->pauseEventListenersForTarget(this);
  }
  inline virtual void resume() noexcept override {
    GenericWeapon::resume();
    CC_DIRECTOR_->getEventDispatcher()->resumeEventListenersForTarget(this);
  }
  static AtomicBomb* create(
      const std::function<void(cocos2d::Node *)> &weaponExplodedCallback,
      const std::function<void()> &weaponDischargedCallback) noexcept;
  virtual bool init() noexcept override {
    if (!GenericWeapon::initWithFile("atomic_bomb_on.png")) {
      return false;
    }
    this->setTag(static_cast<int>(Kind::AtomicBomb));
    return true;
  }
  virtual void setToInitialState() noexcept override {
    GenericWeapon::setToInitialState();
    this->stop();
    this->setSpriteFrame(
       CC::Sprite::create("atomic_bomb_off.png")->getSpriteFrame());
  // TODO
  //    CC::SpriteFrameCache::getInstance()->getSpriteFrameByName(
  //          "atomic_bomb_off.png"));
  }
  virtual void putIn(
    const CC::Vec2  &position,
    const std::function<void(GenericWeapon*)> &explodeWeapon) noexcept;
  virtual void explode(CC::CallFuncN *onExploded) noexcept override;

 private:
  const std::function<void()> weapon_discharged_callback_;
  AtomicBomb(
      const std::function<void(CC::Node*)> &weaponExplodedCallback,
      const std::function<void()> &weaponDischargedCallback);
};

#endif  // CLASSES_GAMEOBJECT_ATOMICBOMB_H
