/*! \brief This file have the implementation for Package class
    \file package.cc
    \author Copyright owners
    \date 6/12/2014

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

#include "Classes/GameObject/package.h"

bool Package::init() {
  if (!Sprite::initWithFile("parachute_with_healt.png")) {
    return false;
  }
  this->setTag(static_cast<int>(NodeTag::packet));
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  return true;
}

void Package::setToInitialState() noexcept {
  this->stopAllActions();
  this->setOpacity(255);
  this->setScale(1.f);
  this->setVisible(false);
  this->setPositionY(CC_DIRECTOR_->getVisibleSize().height +
                     visible_origin_.y + this->getContentSize().height);
}

void Package::setKind(const Kind &kind) noexcept {
  kind_ = kind;
  CC::SpriteFrame *frame{nullptr};
  if (kind_ == Kind::Healt) {
    frame = CC::Sprite::create("parachute_with_healt.png")->getSpriteFrame();
  } else if (kind_ == Kind::AtomicBomb) {
    frame = CC::Sprite::create(
       "parachute_with_atomic_bomb.png")->getSpriteFrame();
  } else if (kind_ == Kind::Tornado) {
    frame = CC::Sprite::create("parachute_with_tornado.png")->getSpriteFrame();
  }
  if (frame != nullptr) {
    this->setSpriteFrame(frame);
  }
}

void Package::launch(float reverseSpeed) noexcept {
  this->setVisible(true);
  float initX{};
  float endX{};
  G_COLABORATION_->generateTrayectory(&initX, &endX);
  this->setPosition(
      CC::Vec2 {initX,
                CC_DIRECTOR_->getVisibleSize().height +
                this->getContentSize().height*.5f} +
        visible_origin_);
  CC::Sequence *seq{CC::Sequence::create(
      CC::MoveTo::create(2 * reverseSpeed,
                     CC::Vec2{endX, this->getContentSize().height*.5f}),
          CC::CallFunc::create([this](){
      this->setToInitialState();
    }),
      nullptr)};
  seq->setTag(static_cast<int>(ActionTag::falling));
  this->runAction(seq);

  float variance{10*(endX - initX)/CC_DIRECTOR_->getVisibleSize().width};
  CC::FiniteTimeAction* easeSwing{
    CC::Sequence::create(
          CC::EaseInOut::create(CC::RotateTo::create(1.2f, -10 - variance), 2),
          CC::EaseInOut::create(CC::RotateTo::create(1.2f, 10 - variance), 2),
          nullptr)};
  CC::RepeatForever *rfe{
    CC::RepeatForever::create(static_cast<CC::ActionInterval*>(easeSwing))};
  rfe->setTag(static_cast<int>(ActionTag::swing));
  this->runAction(rfe);
}

void Package::puff(CC::Vec2 from) noexcept {
  this->stopActionByTag(static_cast<int>(ActionTag::falling));
  this->stopActionByTag(static_cast<int>(ActionTag::swing));

  CC::Size screenSize{CC_DIRECTOR_->getVisibleSize()};
  CC::Vec2 destin{0.f, screenSize.height*.8f};
  if (this->getPositionX() > from.x) {
    destin.x = this->getPositionX() * 1.6f;
    this->setRotation(10.f);
    this->runAction(CC::RotateTo::create(.4f, 10.f));
  } else {
    destin.x = this->getPositionX() * .6f;
    this->runAction(CC::RotateTo::create(.4f, -10.f));
  }
  CC::Sequence *pf{
    CC::Sequence::create(
          CC::MoveTo::create(1.4f, destin),
          CC::CallFunc::create(std::bind(&Package::setToInitialState,
                                         this)),
          nullptr)};
  pf->setTag(static_cast<int>(ActionTag::puffed));
  this->runAction(pf);
  this->runAction(CC::FadeOut::create(1.f));
  this->runAction(CC::ScaleTo::create(1.f, .3f));
}


