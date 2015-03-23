/*! \brief This file have the implementation for Packages class
    \file packages.cc
    \author Copyright owners
    \date 6/20/2014

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

#include "Classes/GameObject/packages.h"

Packages::Packages(const std::function<void(CC::Node*)> &takenCallback)
  : packages_{}
  , packages_index_{0}
  , pendant_packages_bomb_{0}
  , pendant_packages_atomic_bomb_{0}
  , pendant_packages_tonado_{0}
  , pendant_packages_healt_{0}
  , taken_callback_{takenCallback} {
}

Packages *Packages::create(
    const std::function<void(CC::Node*)> &takenCallback) noexcept {
  Packages *pRet{new Packages{takenCallback}};
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool Packages::init() {
  for (std::int8_t i = 0; i < m_cMaxPackagesNumber; ++i) {
    Package *package{Package::create()};
    this->initListener(package);
    packages_.pushBack(package);
  }
  return true;
}

void Packages::setToInitialState() noexcept {
  for (Package *package : packages_) {
    package->setToInitialState();
  }
  pendant_packages_bomb_ = 0;
  pendant_packages_atomic_bomb_ = 0;
  pendant_packages_tonado_ = 0;
  pendant_packages_healt_ = 0;
}

void Packages::setPendantPackages(Package::Kind package,
                                  std::uint8_t pendant) noexcept {
  switch (package) {
    case Package::Kind::Bomb:
      pendant_packages_bomb_ = pendant;
      break;
    case Package::Kind::AtomicBomb:
      pendant_packages_atomic_bomb_ = pendant;
      break;
    case Package::Kind::Tornado:
      pendant_packages_tonado_ = pendant;
      break;
    case Package::Kind::Healt:
      pendant_packages_healt_ = pendant;
      break;
    default:
      break;
  }
}

const std::uint8_t Packages::pendantPackages(
    Package::Kind package) const  noexcept {
  std::uint8_t pendant{0};
  switch (package) {
    case Package::Kind::Bomb:
      pendant = pendant_packages_bomb_;
      break;
    case Package::Kind::AtomicBomb:
      pendant = pendant_packages_atomic_bomb_;
      break;
    case Package::Kind::Tornado:
      pendant = pendant_packages_tonado_;
      break;
    case Package::Kind::Healt:
      pendant = pendant_packages_healt_;
      break;
    default:
      break;
  }
  return pendant;
}

void Packages::launchNext(
    Package::Kind package_kind, float reverseSpeed) noexcept {
  for (Package *package : packages_) {
    if (!package->getActionByTag(
           static_cast<int>(Package::ActionTag::falling)) &&
        !package->isVisible()) {
      package->setKind(package_kind);
      package->launch(reverseSpeed);
      switch (package_kind) {
        case Package::Kind::Bomb:
          --pendant_packages_bomb_;
          break;
        case Package::Kind::AtomicBomb:
          --pendant_packages_atomic_bomb_;
          break;
        case Package::Kind::Tornado:
          --pendant_packages_tonado_;
          break;
        case Package::Kind::Healt:
          --pendant_packages_healt_;
          break;
        default:
          break;
      }
      break;
    }
  }
}

void Packages::initListener(Package *package) noexcept {
  CC::EventListenerTouchOneByOne* listener{
    CC::EventListenerTouchOneByOne::create()};
  listener->setSwallowTouches(true);
  listener->onTouchBegan = ([](CC::Touch* touch, CC::Event* event) {
    if (G_COLABORATION_->paused()) {
      return false;
    }
    Package *target{
      static_cast<Package*>(event->getCurrentTarget())};
    if (target == nullptr) {
      return false;
    }
    if (target->isVisible() && target->getOpacity() >= 250) {
      CC::Size targetSize{target->getContentSize()};
      CC::Vec2 locationInNode{target->convertToNodeSpace(touch->getLocation())};
      CC::Rect rect{CC::Rect{0, 0, targetSize.width, targetSize.height}};
      if (rect.containsPoint(locationInNode)) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  });
  listener->onTouchEnded = ([&](CC::Touch* touch, CC::Event* event) {
    Package *package{static_cast<Package*>(event->getCurrentTarget())};
    if (package != nullptr) {
      if (package->getKind() != Package::Kind::Healt) {
        G_COLABORATION_->playEffect("package_taken.ogg");
      }
      taken_callback_(package);
    }
  });
  CC_DIRECTOR_->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        listener, package);
}
