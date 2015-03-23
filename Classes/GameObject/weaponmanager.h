/*! \brief This file have the interface for WeaponManager class.
    \file weaponmanager.h
    \author Copyright owners
    \date 7/25/2014

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

#ifndef CLASSES_GAMEOBJECT_WEAPONMANAGER_H
#define CLASSES_GAMEOBJECT_WEAPONMANAGER_H

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/GameObject/atomicbombs.h"
#include "Classes/GameObject/bombs.h"
#include "Classes/GameObject/tornados.h"
#include "Classes/GameObject/package.h"

class WeaponManager : public CC::Ref {
 public:
  ~WeaponManager();

  WeaponManager(const WeaponManager&) = delete;
  WeaponManager& operator=(const WeaponManager&) = delete;

  WeaponManager(const WeaponManager&&) = delete;
  WeaponManager& operator=(const WeaponManager&&) = delete;

  static WeaponManager* create(
      CC::Layer *gameLayer,
      CC::SEL_SCHEDULE updateAtomicBomsCollitions,
      CC::SEL_SCHEDULE updateBombsCollitions,
      CC::SEL_SCHEDULE updateTornadosCollitions,
      const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept;

  bool init(
    CC::Layer *gameLayer,
    CC::SEL_SCHEDULE updateAtomicBomsCollitions,
    CC::SEL_SCHEDULE updateBombsCollitions,
    CC::SEL_SCHEDULE updateTornadosCollitions,
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept;
  void putWeaponIn(const CC::Vec2& position) noexcept;

  void setToInitialState() noexcept;
  inline void pause() noexcept {
    atomic_bombs_->pause();
    bombs_->pause();
    tornados_->pause();
  }
  inline void stop() noexcept {
    atomic_bombs_->stop();
    bombs_->stop();
    tornados_->stop();
  }
  inline void resume() noexcept {
    atomic_bombs_->resume();
    bombs_->resume();
    tornados_->resume();
  }
  inline void setVisible(bool visible) noexcept {
    atomic_bomb_weapon_selector_->setVisible(visible);
    bomb_weapon_selector_->setVisible(visible);
    tornado_weapon_selector_->setVisible(visible);
  }
  void setSelectedWeapon(GenericWeapon::Kind selected) noexcept;
  inline const AtomicBombs* atomicBombs() const noexcept {
    return atomic_bombs_;
  }

  const Bombs* bombs() const  noexcept {
    return bombs_;
  }

  const Tornados* tornados() const  noexcept {
    return tornados_;
  }

  void explodeWeapon(const AtomicBomb *weapon)  noexcept {
    atomic_bombs_->explodeWeapon(weapon);
  }

  void explodeWeapon(const Bomb *weapon) noexcept {
    bombs_->explodeWeapon(weapon);
  }

  void explodeWeapon(const Tornado *weapon)  noexcept {
    tornados_->explodeWeapon(weapon);
  }

  const CC::Sprite* atomicBombWeaponSelector() const  noexcept {
    return atomic_bomb_weapon_selector_;
  }

  const CC::Sprite* bombWeaponSelector() const noexcept {
    return bomb_weapon_selector_;
  }

  const CC::Sprite* tornadoWeaponSelector() const noexcept {
    return tornado_weapon_selector_;
  }


  const CC::Label* atomicBombWeaponSelectorLabel() const noexcept;

  const CC::Label* bombWeaponSelectorLabel() const noexcept;

  const CC::Label* tornadoWeaponSelectorLabel() const noexcept;
  void increaseOneCurrentWeaponsNumber(
    const Package::Kind& weaponKind) noexcept;


 private:
  enum class WeaponSelectorTag  {
    Bomb,
    AtomicBomb,
    Tornado
  };

  AtomicBombs *atomic_bombs_;
  Bombs *bombs_;
  GenericWeapon::Kind selected_weapon_;
  Tornados *tornados_;
  CC::Vec2 visible_origin_;
  CC::Size visible_size_;
  CC::Sprite *atomic_bomb_weapon_selector_;
  CC::Label *atomic_bomb_weapon_selector_label_;
  CC::Sprite *bomb_weapon_selector_;
  CC::Label *bomb_weapon_selector_label_;
  CC::Sprite *tornado_weapon_selector_;
  CC::Label *tornado_weapon_selector_label_;

  WeaponManager();
  /*!
   * \brief updateAvaliableWeaponsLabel
   * \brief Para los tornados y bombas atomicas el _currentWeaponsNumber no se
   * \brief actualiza en realidad hasta que el arma sea activada por eso hay que
   * \brief restar los que estan activados para que en base a esto se muestre
   * \brief la cantidad.
   * \sa GenericWeapons::exploded(CC::Node* pSender),
   * \sa Tornados::putWeaponIn(const CC::Vec2 position),
   * \sa AtomicBombs::putWeaponIn(const CC::Vec2 position),
   * \sa Bombs::putWeaponIn(const CC::Vec2 position)
   */
  void updateAvaliableWeaponsLabel(const GenericWeapon::Kind& updated) noexcept;
  void createWeaponSelector() noexcept;
};

#endif  // CLASSES_GAMEOBJECT_WEAPONMANAGER_H
