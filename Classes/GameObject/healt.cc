/*! \brief This file have the implementation for Healt class.
    \file healt.cc
    \author Copyright owners
    \date 10/21/2014

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

#include "Classes/GameObject/healt.h"

Healt::Healt()
  : healt_{100}
  , healt_background_{nullptr} {
}

bool Healt::init() {
  if (!CC::Sprite::initWithFile(
        "Animations/healt_progress_bar/healt_value_100.png")) {
    return false;
  }
  this->setAnchorPoint(CC::Vec2::ANCHOR_TOP_LEFT);
  this->setPosition(
        CC::Vec2 {CC_DIRECTOR_->getVisibleSize().width*.38f,
                  CC_DIRECTOR_->getVisibleSize().height} +
          CC_DIRECTOR_->getVisibleOrigin());
  return true;
}

void Healt::set_healt(const std::uint8_t& healt) noexcept {
  healt_ = healt;
  if (healt_ > 100) {
      healt_ = 100;
  }
  this->stopAllActions();
  if (healt_ <= 10) {
    this->runAction(CC::RepeatForever::create(CC::Blink::create(1.f, 3)));
  } else if (healt_ <= 30) {
    this->runAction(CC::RepeatForever::create(CC::Blink::create(1.f, 2)));
  }
  this->updatePrograsBar();
}

void Healt::updatePrograsBar() noexcept {
  std::uint8_t sufix{0u};
  if (healt_ < 10) {
    sufix = 0u;
  } else if (healt_ < 20) {
    sufix = 10u;
  } else if (healt_ < 30) {
    sufix = 20u;
  } else if (healt_ < 40) {
    sufix = 30u;
  } else if (healt_ < 50) {
    sufix = 40u;
  } else if (healt_ < 60) {
    sufix = 50u;
  } else if (healt_ < 70) {
    sufix = 60u;
  } else if (healt_ < 80) {
    sufix = 70u;
  } else if (healt_ < 90) {
    sufix = 80u;
  } else if (healt_ < 100) {
    sufix = 90u;
  } else {
    sufix = 100u;
  }
  if (sufix == 0u) {
    this->stopAllActions();
    this->setVisible(false);
  } else {
    this->setVisible(true);
    char path[200]{0};
    std::snprintf(
          path,
          sizeof(path),
          "Animations/healt_progress_bar/healt_value_%u.png",
          sufix);
    this->setSpriteFrame(CC::Sprite::create(path)->getSpriteFrame());
  }
}
