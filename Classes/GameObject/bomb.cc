/*! \brief This file have the implementation for Bomb class
    \file bomb.cc
    \author Copyright owners
    \date 6/7/2014

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

#include "Classes/GameObject/bomb.h"

Bomb::Bomb(const std::function<void(CC::Node *)> &weaponExplodedCallback)
  : GenericWeapon{weaponExplodedCallback}
  , sprite_frame_name_{"bomb_on.png"}
  , medal_{nullptr}
  , medal_label_{nullptr} {
}

Bomb* Bomb::create(
    const std::function<void(CC::Node *)> &weaponExplodedCallback) {
  Bomb* pRet{new Bomb(weaponExplodedCallback)};
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

bool Bomb::init() noexcept {
  if (!GenericWeapon::initWithFile(sprite_frame_name_)) {
    return false;
  }
  this->setTag(static_cast<int>(Kind::Bomb));
  medal_ = CC::Sprite::create("gold_medal.png");
  medal_label_ = CC::Label::createWithTTF(
     "0",
     "fonts/Dosis-Light.otf",
     CC_DIRECTOR_->getVisibleSize().height*.06f);
  medal_label_->setColor(CC::Color3B{255, 25, 92});
  medal_->addChild(medal_label_);
  medal_label_->setPosition(CC::Vec2{medal_->getContentSize().width*.5f,
                                     medal_->getContentSize().height*.63f});
  return true;
}

void Bomb::putIn(
  const CC::Vec2  &position,
  const std::function<void(GenericWeapon*)>& explodeWeapon) noexcept {
  GenericWeapon::putIn(position);
  this->setSpriteFrame(CC::Sprite::create("bomb_off.png")->getSpriteFrame());
  this->runAction(
        CC::Sequence::create(
          CC::ScaleTo::create(3.5f, 1.f),
          CC::DelayTime::create(3.f),
          CC::CallFunc::create([this, &explodeWeapon](){
    explodeWeapon(this);
  }),
          nullptr));
  CC::Sequence *seq{
      CC::Sequence::createWithTwoActions(
        CC::DelayTime::create(1),
        CC::CallFunc::create(CC_CALLBACK_0(GenericWeapon::activeBomb, this)))};
  this->runAction(seq);
}

void Bomb::explode(CC::CallFuncN* onExploded) noexcept {
  GenericWeapon::explode();
  this->setSpriteFrame(
     CC::Sprite::create("Animations/bobm_explosion.png")->getSpriteFrame());
  float scaleFactor {this->getBoundingBox().size.width /
      this->getContentSize().width};
  this->setScale(.3f);
  G_COLABORATION_->playEffect("bomb.ogg");
  CC::Sequence *seq = CC::Sequence::create(
      CC::ScaleTo::create(scaleFactor * explotion_duration_,
                          scaleFactor * 1.1f),
      onExploded,
      nullptr);
  seq->setTag(static_cast<int>(Tag::explodin));
  this->set_is_exploding(true);
  this->runAction(seq);
}

void Bomb::loot(const std::function<void(int)>& increaseScore,
                const CC::Vec2& pos) noexcept {
  if (neutralized_menaces_by_last_shoot_ > 1) {
    medal_->runAction(CC::Sequence::createWithTwoActions(
                      CC::DelayTime::create(1.6f),
                      CC::CallFunc::create([]{
      G_COLABORATION_->playEffect("medall.ogg");
    })));
    medal_->setPosition(this->getPosition());
    medal_->setVisible(true);
    if (neutralized_menaces_by_last_shoot_ == 2u) {
      medal_->setSpriteFrame(
         CC::Sprite::create("silver_medal.png")->getSpriteFrame());
    } else {
      medal_->setSpriteFrame(
         CC::Sprite::create("gold_medal.png")->getSpriteFrame());
    }
    medal_->setVisible(true);
    medal_->setScale(1.f);
    medal_->runAction(
          CC::Sequence::create(CC::DelayTime::create(1.2f),
                               CC::ScaleTo::create(1.3f, .1f),
                               nullptr));
    std::int32_t val{150 * (neutralized_menaces_by_last_shoot_ - 1)};
    medal_label_->setString(std::to_string(val));
    medal_->runAction(
          CC::Sequence::create(
            CC::DelayTime::create(.6f),
            CC::EaseElasticIn::create(CC::MoveTo::create(1.3f, pos), .6f),
            CC::CallFunc::create([this, &increaseScore, val]{
      medal_->setVisible(false);
      increaseScore(val);
    }),
            nullptr));
  }
}
