/*! \brief This file have the implementation for Asteroid class.
    \file asteroid.cc
    \author Copyright owners
    \date 12/8/2014

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

#include "Classes/GameObject/asteroid.h"

Asteroid::Asteroid()
  : destiny_ {CC::Vec2::ZERO}
  , visible_size_{CC::Size::ZERO}
  , num_{nullptr}
  , act_rot_{nullptr}
  , visible_origin_{CC::Vec2::ZERO} {
}

Asteroid::~Asteroid() {
  CC_SAFE_RELEASE(act_rot_);
}


Asteroid* Asteroid::create(const std::string &filename) noexcept {
  Asteroid *ast{new Asteroid()};
  if (ast && ast->initWithFile(filename)) {
    ast->autorelease();
    return ast;
  } else {
    CC_SAFE_DELETE(ast);
    ast = nullptr;
    return nullptr;
  }
}

bool Asteroid::initWithFile(const std::string &filename) noexcept {
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  visible_size_ = CC_DIRECTOR_->getVisibleSize();
  // FIXME(denisacostaq@gmail.com): asegurarme que aqui el resze de la fuente
  // para cada resolucion LabelBMFont tiene muchos mas editores para crear la
  // fuente pero parece mas lento
  num_ = CC::Label::createWithTTF("+15",
                                  "fonts/Dosis-Light.otf",
                                  visible_size_.height*.065f);
  num_->enableOutline(CC::Color4B{204, 84, 0, 255},
                      num_->getContentSize().height*.05f);
  num_->setColor(CC::Color3B{0, 255, 0});
  this->createAnimations();
  return Sprite::initWithFile(filename);
}

void Asteroid::setToInitialState() noexcept {
  this->stopAllActions();
  this->setVisible(false);
  this->num_->setOpacity(0);
  this->stopAllActions();
  /*NOTE:
  Esto es un error, yo le digo a la animacio que me restablesca el frame
  pero si se convina el explosion con el move no me lo hace
  m_animMetExp->setRestoreOriginalFrame (true);*/
  this->setSpriteFrame(CC::Sprite::create("asteroid_2.png")->getSpriteFrame());
  this->setPositionY(CC_DIRECTOR_->getVisibleSize().height + visible_origin_.y +
                     this->getContentSize().height);
}

void Asteroid::launch(
    float reverseSpeed,
    CC::CallFuncN *callback) noexcept {
  // FIXME(denisacostaq) detener todas las acciones
  if (this->getActionByTag(static_cast<int>(ActTag::moveTo))) {
    return;
  }
  float initX{0.f};
  float endX{0.f};
  G_COLABORATION_->generateTrayectory(&initX, &endX);
  this->setPosition(
        CC::Vec2{initX,
                 visible_size_.height + this->getContentSize().height*.5f});
  this->setVisible(true);
  CC::Sequence *seq{
    CC::Sequence::create(
          CC::MoveTo::create(reverseSpeed,
                             CC::Vec2{endX, this->getContentSize().height*.5f}),
          callback,
          nullptr)};
  seq->setTag(static_cast<int>(ActTag::moveTo));
  this->runAction(seq);
  CC::Action *cln_act_rot{act_rot_->clone()};
  cln_act_rot->setTag(static_cast<int>(ActTag::rotation));
  this->runAction(cln_act_rot);
  this->setDestiny(CC::Vec2{endX, this->getContentSize().height*.5f});
}


void Asteroid::destroy(CC::CallFuncN *callback) noexcept {
  this->stopActionByTag(static_cast<int>(ActTag::rotation));
  if (!CC::Animate::create(
       CC::AnimationCache::getInstance()->getAnimation("animMetExp"))) {
    CC::log("exit(0);");
    exit(0);
  }
  CC::Sequence* groundHit = CC::Sequence::create(
        CC::Animate::create(
          CC::AnimationCache::getInstance()->getAnimation("animMetExp")),
        callback,
        nullptr);
  groundHit->setTag(static_cast<int>(ActTag::explosion));
  this->runAction(groundHit);
  // NOTE(denisacostaq@gmail.com): Aqui no hace falta desplazar del origin
  // porque this lo tiene.
  num_->setPosition(this->getPosition());
  this->num_->setOpacity(255);
  CC::MoveBy *mb{
    CC::MoveBy::create(
          1.f, CC::Vec2{num_->getContentSize().width*.5f, 0.f})};
  num_->runAction(
        CC::Repeat::create(
          CC::Sequence::createWithTwoActions(mb->clone(),
                                             mb->clone()->reverse()),
          5));
  num_->runAction(CC::MoveBy::create(5.f, CC::Vec2{0.f, 100.f}));
  num_->runAction(CC::FadeOut::create(5.f));
}

void Asteroid::impact() noexcept {
  this->stopActionByTag(static_cast<int>(ActTag::rotation));
  this->setRotation(0.f);
  // WARNING: Asi esta bien pero al cambiar la imagen pudiera cambiar
  // el factor.
  setPositionY(getPositionY() + 3 * getContentSize().height);
  CC::Sequence* impact{
    CC::Sequence::createWithTwoActions(
          CC::Animate::create(
            CC::AnimationCache::getInstance()->getAnimation("animMetImp")),
          CC::MoveTo::create(
            0.f, CC::Vec2{.6, -this->getContentSize().height * 5.f}))};
  // FIXME(denis): impact->setTag (static_cast<int>(ActionTag::meteorImpact));
  this->runAction(impact);
  G_COLABORATION_->playEffect("asteroid.ogg");
}
