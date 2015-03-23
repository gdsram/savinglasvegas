/*! \brief This file have the implementation for WeaponManager class.
    \file weaponmanager.cc
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

#include "Classes/GameObject/weaponmanager.h"

WeaponManager::WeaponManager()
  : atomic_bombs_{nullptr}
  , bombs_{nullptr}
  , selected_weapon_{GenericWeapon::Kind::Bomb}
  , tornados_{nullptr}
  , visible_origin_{CC::Vec2::ZERO}
  , visible_size_{CC::Size::ZERO}
  , atomic_bomb_weapon_selector_{nullptr}
  , atomic_bomb_weapon_selector_label_{nullptr}
  , bomb_weapon_selector_{nullptr}
  , bomb_weapon_selector_label_{nullptr}
  , tornado_weapon_selector_{nullptr}
  , tornado_weapon_selector_label_{nullptr} {
}

WeaponManager::~WeaponManager() {
  CC_SAFE_RELEASE(tornados_);
  CC_SAFE_RELEASE(bombs_);
  CC_SAFE_RELEASE(atomic_bombs_);
}

WeaponManager* WeaponManager::create(
    CC::Layer *gameLayer,
    CC::SEL_SCHEDULE updateAtomicBomsCollitions,
    CC::SEL_SCHEDULE updateBombsCollitions,
    CC::SEL_SCHEDULE updateTornadosCollitions,
    const std::function<void(cocos2d::Node *)> &weaponExplodedCallback)
      noexcept {
  WeaponManager *pRet{new WeaponManager{}};
  if (pRet && pRet->init(gameLayer,
                         updateAtomicBomsCollitions,
                         updateBombsCollitions,
                         updateTornadosCollitions,
                         weaponExplodedCallback)) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool WeaponManager::init(
    CC::Layer *gameLayer,
    CC::SEL_SCHEDULE updateAtomicBomsCollitions,
    CC::SEL_SCHEDULE updateBombsCollitions,
    CC::SEL_SCHEDULE updateTornadosCollitions,
    const std::function<void(CC::Node*)>& weaponExplodedCallback) noexcept {
  visible_size_ = CC_DIRECTOR_->getVisibleSize();
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();

  atomic_bombs_ = AtomicBombs::create(gameLayer,
                                     updateAtomicBomsCollitions,
                                     weaponExplodedCallback);
  atomic_bombs_->retain();
  bombs_ = Bombs::create(gameLayer,
                         updateBombsCollitions,
                         weaponExplodedCallback);
  bombs_->retain();
  tornados_ = Tornados::create(gameLayer,
                               updateTornadosCollitions,
                               weaponExplodedCallback);
  tornados_->retain();

  createWeaponSelector();

  return true;
}

void WeaponManager::setToInitialState() noexcept {
  bombs_->setToInitialState();
  tornados_->setToInitialState();
  atomic_bombs_->setToInitialState();
  this->setVisible(true);
  atomic_bomb_weapon_selector_->setVisible(false);
  tornado_weapon_selector_->setVisible(false);
  this->setSelectedWeapon(GenericWeapon::Kind::Bomb);
  updateAvaliableWeaponsLabel(GenericWeapon::Kind::AtomicBomb);
  updateAvaliableWeaponsLabel(GenericWeapon::Kind::Bomb);
  updateAvaliableWeaponsLabel(GenericWeapon::Kind::Tornado);
}

void WeaponManager::increaseOneCurrentWeaponsNumber(
    const Package::Kind &weaponKind) noexcept {
  switch (weaponKind) {
    case Package::Kind::Bomb:
      bombs_->increaseOneCurrentWeaponsNumber();
      this->updateAvaliableWeaponsLabel(GenericWeapon::Kind::Bomb);
      break;
    case Package::Kind::AtomicBomb:
      atomic_bomb_weapon_selector_->setVisible(true);
      atomic_bomb_weapon_selector_label_->setVisible(true);
      atomic_bombs_->increaseOneCurrentWeaponsNumber();
      this->updateAvaliableWeaponsLabel(GenericWeapon::Kind::AtomicBomb);
      break;
    case Package::Kind::Tornado:
      tornado_weapon_selector_->setVisible(true);
      tornado_weapon_selector_label_->setVisible(true);
      tornados_->increaseOneCurrentWeaponsNumber();
      this->updateAvaliableWeaponsLabel(GenericWeapon::Kind::Tornado);
      break;
    default:
      break;
  }
}

void WeaponManager::setSelectedWeapon(GenericWeapon::Kind selected) noexcept {
  switch (selected) {
    case GenericWeapon::Kind::Bomb:
      bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_bomb_on.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      atomic_bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_atomic_bomb_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      tornado_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_tornado_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      break;
    case GenericWeapon::Kind::AtomicBomb:
      bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_bomb_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      atomic_bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_atomic_bomb_on.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      tornado_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_tornado_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      break;
    case GenericWeapon::Kind::Tornado:
      bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_bomb_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      atomic_bomb_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_atomic_bomb_off.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      tornado_weapon_selector_->setSpriteFrame(
            CC::Sprite::create(
              "Buttons/weapon_selector_and_discharger/weapon_selector_tornado_on.png")->getSpriteFrame());  // NOLINT(whitespace/line_length)
      break;
    default:
      break;
  }
  selected_weapon_ = selected;
  updateAvaliableWeaponsLabel(selected);
}

void WeaponManager::putWeaponIn(const CC::Vec2& position) noexcept {
  switch (selected_weapon_) {
    case GenericWeapon::Kind::Bomb:
      bombs_->putWeaponIn(position);
      updateAvaliableWeaponsLabel(GenericWeapon::Kind::Bomb);
      break;
    case GenericWeapon::Kind::AtomicBomb:
      atomic_bombs_->putWeaponIn(position);
      updateAvaliableWeaponsLabel(GenericWeapon::Kind::AtomicBomb);
      break;
    case GenericWeapon::Kind::Tornado:
      tornados_->putWeaponIn(position);
      updateAvaliableWeaponsLabel(GenericWeapon::Kind::Tornado);
      break;
    default:
      break;
  }
}

// FIXME: en reaccion en cadena explotar las bombas solo si esta lista

void WeaponManager::updateAvaliableWeaponsLabel(
    const GenericWeapon::Kind& updated) noexcept {
  std::uint8_t count{0};
  std::uint8_t currentWeaponsNumber{0};
  CC::Vector<GenericWeapon*> weapons;
  switch (updated) {
    case GenericWeapon::Kind::Bomb:
      count = bombs_->currentWeaponsNumber();
      bomb_weapon_selector_label_->setString(std::to_string(count));
      break;
    case GenericWeapon::Kind::AtomicBomb:
      count = atomic_bombs_->currentWeaponsNumber();
      weapons =
        const_cast<CC::Vector<GenericWeapon*>&>(atomic_bombs_->weapons());
      currentWeaponsNumber = atomic_bombs_->currentWeaponsNumber();
      for (std::uint8_t i{0}; i < currentWeaponsNumber; ++i) {
        if (weapons.at(i)->isVisible()) {
          --count;
        }
      }
      atomic_bomb_weapon_selector_label_->setString(std::to_string(count));
      break;
    case GenericWeapon::Kind::Tornado:
      count = tornados_->currentWeaponsNumber();
      weapons = const_cast<CC::Vector<GenericWeapon*>&>(tornados_->weapons());
      currentWeaponsNumber = tornados_->currentWeaponsNumber();
      for (std::uint8_t i{0}; i < currentWeaponsNumber; ++i) {
        if (weapons.at(i)->isVisible()) {
          --count;
        }
      }
      tornado_weapon_selector_label_->setString(std::to_string(count));
      break;
    default:
      break;
  }
}

void WeaponManager::createWeaponSelector() noexcept {
  CC::EventListenerTouchOneByOne* listener{
    CC::EventListenerTouchOneByOne::create()};
  listener->setSwallowTouches(true);
  listener->onTouchBegan = ([](CC::Touch* touch, CC::Event* event) {
    if (G_COLABORATION_->paused()) {
      return false;
    }
    CC::Sprite *target{static_cast<CC::Sprite*>(event->getCurrentTarget())};
    if (target == nullptr) {
      return false;
    }
    if (target->isVisible() == false) {
      return false;
    }
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

  bomb_weapon_selector_ = CC::Sprite::create(
          "Buttons/weapon_selector_and_discharger/weapon_selector_bomb_on.png");
  bomb_weapon_selector_->setTag(static_cast<int>(WeaponSelectorTag::Bomb));
  bomb_weapon_selector_->setPosition(
        CC::Vec2 {visible_size_.width -
                    bomb_weapon_selector_->getContentSize().width*.5f,
                  visible_size_.height*.1f} +
          visible_origin_);
  listener->onTouchEnded = ([&](CC::Touch* touch, CC::Event* event){
      CC::Sprite *target = static_cast<CC::Sprite*>(event->getCurrentTarget());
      target->setOpacity(255);
      this->setSelectedWeapon(GenericWeapon::Kind::Bomb);
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener->clone(),
        bomb_weapon_selector_);
  bomb_weapon_selector_label_ = CC::Label::createWithTTF(
        "0",
        "fonts/Dosis-Light.otf",
        visible_size_.height*.05f);
  bomb_weapon_selector_->addChild(bomb_weapon_selector_label_);
  bomb_weapon_selector_label_->setPosition(
        CC::Vec2{bomb_weapon_selector_->getContentSize().width*.82f,
                 bomb_weapon_selector_->getContentSize().height*.25f});

  tornado_weapon_selector_ =
    CC::Sprite::create(
      "Buttons/weapon_selector_and_discharger/weapon_selector_tornado_on.png");
  tornado_weapon_selector_->setPosition(
        CC::Vec2 {
          visible_size_.width -
            tornado_weapon_selector_->getContentSize().width*.5f,
          visible_size_.height*.35f} +
          visible_origin_);
  listener->onTouchEnded = ([&](CC::Touch* touch, CC::Event* event) {
    CC::Sprite* target{static_cast<CC::Sprite*>(event->getCurrentTarget())};
    target->setOpacity(255);
    this->setSelectedWeapon(GenericWeapon::Kind::Tornado);
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener->clone(),
        tornado_weapon_selector_);
  tornado_weapon_selector_label_ = CC::Label::createWithTTF(
        "0",
        "fonts/Dosis-Light.otf",
        visible_size_.height*.05f);
  tornado_weapon_selector_->addChild(tornado_weapon_selector_label_);
  tornado_weapon_selector_label_->setPosition(
        CC::Vec2{tornado_weapon_selector_->getContentSize().width*.82f,
                 tornado_weapon_selector_->getContentSize().height*.25f});

  atomic_bomb_weapon_selector_ = CC::Sprite::create(
     "Buttons/weapon_selector_and_discharger/weapon_selector_atomic_bomb_on.png");  // NOLINT(whitespace/line_length)
  atomic_bomb_weapon_selector_->setPosition(
        CC::Vec2 {
          visible_size_.width -
            atomic_bomb_weapon_selector_->getContentSize().width*.5f,
          visible_size_.height*.6f} +
          visible_origin_);
  atomic_bomb_weapon_selector_->setTag(
        static_cast<int>(WeaponSelectorTag::AtomicBomb));
  listener->onTouchEnded = ([&](CC::Touch* touch, CC::Event* event){
      CC::Sprite *target = static_cast<CC::Sprite*>(event->getCurrentTarget());
      target->setOpacity(255);
      this->setSelectedWeapon(GenericWeapon::Kind::AtomicBomb);
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener->clone(),
        atomic_bomb_weapon_selector_);
  atomic_bomb_weapon_selector_label_ = CC::Label::createWithTTF(
        "0",
        "fonts/Dosis-Light.otf",
        visible_size_.height*.05f);
  atomic_bomb_weapon_selector_->addChild(atomic_bomb_weapon_selector_label_);
  atomic_bomb_weapon_selector_label_->setPosition(
        CC::Vec2{atomic_bomb_weapon_selector_->getContentSize().width*.82f,
                 atomic_bomb_weapon_selector_->getContentSize().width*.25f});
}
