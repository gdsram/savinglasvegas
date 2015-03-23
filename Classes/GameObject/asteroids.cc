/*! \brief This file have the implementation for Asteroids class
    \file asteroids.cc
    \author Copyright owners
    \date 5/27/2014

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

#include "Classes/GameObject/asteroids.h"

Asteroids::Asteroids()
  : mvisible_size_{CC::Size::ZERO}
  , meteors_index_{0}
  , visible_origin_{CC::Vec2::ZERO} {
}

void Asteroids::initMeteors() noexcept {
  for (int i = 0; i < cMaxMeteorsNumber_; i++) {
    Asteroid *meteor = static_cast<Asteroid*>(
          Asteroid::create("asteroid_2.png"));
    meteor->setPosition(CC::Vec2 {mvisible_size_.width*.5f,
                                  mvisible_size_.height*.5f} +
                          visible_origin_);
    meteor->setVisible(false);
    meteor->setTag(static_cast<int>(NodeTag::meteor));
    meteors_.pushBack(meteor);
  }
  for (Asteroid *m : meteors_) {
    numss_.pushBack(const_cast<CC::Label*>(m->num()));
  }
}

void Asteroids::initAnimations() noexcept {
  char szName[100] = {0};
  CC::Animation *animMetImp{CC::Animation::create()};
  animMetImp->retain();
  for (std::uint8_t i{1}; i <= 14; ++i) {
      std::snprintf(
            szName,
            sizeof(szName),
            "Animations/asteroid_impact/asteroid_impact_%u.png",
            i);
      animMetImp->addSpriteFrame(CC::Sprite::create(szName)->getSpriteFrame());
  }
  animMetImp->setDelayPerUnit(1.5f / 14.f);
  animMetImp->setRestoreOriginalFrame(true);
  CC::AnimationCache::getInstance()->addAnimation(animMetImp, "animMetImp");
  CC_SAFE_RELEASE(animMetImp);

  CC::Animation *animMetExp{CC::Animation::create()};
  animMetExp->retain();
  for (int i{1}; i <= 6; ++i) {
    std::snprintf(
          szName,
          sizeof(szName),
          "Animations/asteroid_explosion/asterois_explosion_%u.png",
          i);
    animMetExp->addSpriteFrame(CC::Sprite::create(szName)->getSpriteFrame());
  }
  animMetExp->setDelayPerUnit(1.f/6.0f);
  animMetExp->setRestoreOriginalFrame(true);
  CC::AnimationCache::getInstance()->addAnimation(animMetExp, "animMetExp");
  CC_SAFE_RELEASE(animMetExp);
}
