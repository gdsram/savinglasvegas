/*! \brief This file have the interface for AtomicBombs class
    \file atomicbombs.h
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

#ifndef CLASSES_GAMEOBJECT_ATOMICBOMBS_H
#define CLASSES_GAMEOBJECT_ATOMICBOMBS_H

#include "Classes/appmacros.h"
#include "Classes/GameObject/atomicbomb.h"
#include "Classes/GameObject/genericweapons.h"

class AtomicBombs : public GenericWeapons {
 public:
  ~AtomicBombs() = default;

  AtomicBombs(const AtomicBombs&) = delete;
  AtomicBombs& operator=(const AtomicBombs&) = delete;

  AtomicBombs(const AtomicBombs&&) = delete;
  AtomicBombs& operator=(const AtomicBombs&&) = delete;

  static AtomicBombs* create(
      CC::Layer *gameLayer,
      CC::SEL_SCHEDULE collitioner_checker,
      const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept;
  bool init(
      const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept;
  inline virtual void setToInitialState() noexcept {
    lighting_read_alarm_->setVisible(false);
    for (GenericWeapon *bomb : weapons_) {
      static_cast<AtomicBomb*>(bomb)->setToInitialState();
    }
    current_weapons_number_ = 0;
    GenericWeapons::setToInitialState();
  }
  inline CC::LayerColor* lightingReadAlarm() const noexcept {
    return lighting_read_alarm_;
  }
  virtual void putWeaponIn(const CC::Vec2 position) noexcept override;
  inline void exploded(CC::Node *pSender) noexcept override {
    GenericWeapons::exploded(pSender);
  }
  inline void pause() noexcept override {
    GenericWeapons::pause();
    CC_DIRECTOR_->getActionManager()->pauseTarget(lighting_read_alarm_);
  }
  inline void resume() noexcept override {
    GenericWeapons::resume();
    CC_DIRECTOR_->getActionManager()->resumeTarget(lighting_read_alarm_);
  }
  inline void stop() noexcept override {
    GenericWeapons::stop();
    lighting_read_alarm_->stopAllActions();
  }

 private:
  const std::function<void(GenericWeapon*)> explode_weapon_;
  CC::LayerColor *lighting_read_alarm_;

  AtomicBombs(CC::Layer *gameLayer, CC::SEL_SCHEDULE collitionerChecker);
  void atomicBombDischargedCallback() noexcept;
};

#endif  // CLASSES_GAMEOBJECT_ATOMICBOMBS_H
