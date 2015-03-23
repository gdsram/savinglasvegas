/*! \brief This file have the implementation for Tornado class.
    \file tornado.cc
    \author Copyright owners
    \date 7/11/2014

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

#include "Classes/GameObject/tornado.h"

Tornado::Tornado(
    const std::function<void(cocos2d::Node *)> &weaponExplodedCallback)
  : GenericWeapon{weaponExplodedCallback} {
}

Tornado* Tornado::create(
    const std::function<void(CC::Node *)> &weaponExplodedCallback) noexcept {
  Tornado* pRet{new Tornado{weaponExplodedCallback}};
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool Tornado::init() {
  if (!Sprite::initWithFile(
        "Animations/tornado_puffing/tornado_puffing_1.png")) {
    return false;
  }
  this->setTag(static_cast<int>(Kind::Tornado));
  return true;
}

void Tornado::setToInitialState() noexcept {
  GenericWeapon::setToInitialState();
  this->stop();
  this->setSpriteFrame(
     CC::Sprite::create(
      "Animations/tornado_puffing/tornado_puffing_1.png")->getSpriteFrame());
}

void Tornado::putIn(const CC::Vec2  &position) noexcept {
  GenericWeapon::putIn(position);
  this->setScale(1.f);
}

void Tornado::explode(CC::CallFuncN* onExploded) noexcept {
  neutralized_menaces_by_last_shoot_ = 0;
  this->stopAllActions();
  float scaleFactor =  this->getBoundingBox().size.width /
      this->getContentSize().width;
  this->setScale(.3f);
  CC::Sequence *seq = CC::Sequence::create(
      CC::ScaleTo::create(scaleFactor * 1.5f, scaleFactor * 1.2f),
      onExploded,
      nullptr);
  seq->setTag(static_cast<int>(Tag::explodin));
  CC::Animate *exp{
    CC::Animate::create(CC::AnimationCache::getInstance()->getAnimation(
                      "TornadoAnimation"))};
  this->runAction(CC::Repeat::create(exp, 4));
  this->set_is_exploding(true);
  this->runAction(seq);
}
