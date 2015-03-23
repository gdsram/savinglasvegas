/*! \brief This file have the implementation for Cloud class
    \file cloud.cc
    \author Copyright owners
    \date 6/27/2014

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

#include "Classes/GameObject/cloud.h"

Cloud::Cloud()
  : visible_size_{CC::Size::ZERO} {
}

bool Cloud::init() {
  char clou_image[250]{0};
  std::random_device rd;
  std::default_random_engine e{rd()};
  std::uniform_int_distribution<int> uniform_dist{1, 2};
  std::snprintf(clou_image,
                sizeof(clou_image),
                "cloud_%d.png",
                uniform_dist(e));
  if (!Sprite::initWithFile(std::string{clou_image})) {
    return false;
  }

  for (int i = 0; i < 4; ++i) {
    fragments_.pushBack(
          CC::Sprite::create("cloud_fragment.png"));
  }

  visible_size_ = CC_DIRECTOR_->getVisibleSize();
  return true;
}

void Cloud::launch(const float &offsetX, const float &offsetY) noexcept {
  static std::random_device rd;
  static std::default_random_engine e{rd()};
  static std::uniform_real_distribution<float> uniform_dist{.5f, 1.f};
  float altitude_factor{uniform_dist(e)};
  this->setPosition(
        CC::Vec2{offsetX - this->getContentSize().width*.5f,
                 offsetY + visible_size_.height * altitude_factor});
  this->setVisible(true);
  CC::Sequence *seq{
    CC::Sequence::createWithTwoActions(
      CC::MoveTo::create((visible_size_.width*.15f) * altitude_factor,
                         CC::Vec2{visible_size_.width*1.5f +
                                    this->getContentSize().width*.5f,
                                  offsetY +
                                    visible_size_.height*altitude_factor}),
      CC::CallFuncN::create([](CC::Node *pSender){
        pSender->setVisible(false);
      }))};
  seq->setTag(static_cast<int>(ActionTag::passing));
  this->runAction(seq);
}

void Cloud::puff(const CC::Vec2& from) noexcept {
  for (CC::Sprite *fragment : fragments_) {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_real_distribution<float> distrb{
      this->getContentSize().width*-.4f,
      this->getContentSize().height*.4f};
    fragment->setPosition(
          this->getPosition() + CC::Vec2{distrb(e), distrb(e)});
    // FIXME: warning aki
    float objectDistance{
      static_cast<float>(
            std::sqrt(
              std::pow((fragment->getPositionX() - from.x), 2) +
                std::pow((fragment->getPositionY() - from.y), 2)))};
    float objectDistanceXComponent{fragment->getPositionX() - from.x};
    CC::Size visibleSize{CC_DIRECTOR_->getVisibleSize()};
    float offset{visibleSize.width * .6f};
    float offsetXComponent{(objectDistanceXComponent*offset)/objectDistance};
    CC::Vec2 destine{CC::Vec2::ZERO};
    destine.x = fragment->getPositionX() + offsetXComponent;
    float m{(from.y - fragment->getPositionY()) /
            (from.x - fragment->getPositionX())};
    float n{from.y - m * from.x};
    destine.y = m * destine.x + n;
    CC::MoveTo *mt{CC::MoveTo::create(4, destine)};
    mt->setTag(static_cast<int>(ActionTag::puffed));
    fragment->runAction(mt);
    fragment->runAction(
          CC::Sequence::createWithTwoActions(CC::FadeIn::create(2.f),
                                             CC::FadeOut::create(2.f)));
  }

  float objectDistance{
    static_cast<float>(
          std::sqrt(
            std::pow((this->getPositionX() - from.x), 2) +
              std::pow((this->getPositionY() - from.y), 2)))};
  float objectDistanceXComponent{this->getPositionX() - from.x};
  CC::Size visibleSize{CC_DIRECTOR_->getVisibleSize()};
  float offset{visibleSize.width * .6f};
  float offsetXComponent{(objectDistanceXComponent*offset)/objectDistance};
  CC::Vec2 destine{CC::Vec2::ZERO};
  destine.x = this->getPositionX() + offsetXComponent;
  float m{(from.y - this->getPositionY()) / (from.x - this->getPositionX())};
  float n{from.y - m * from.x};
  destine.y = m * destine.x + n;
  CC::log("}destine %f %f{", destine.x, destine.y);
  CC::MoveTo *mt{CC::MoveTo::create(4, destine)};
  mt->setTag(static_cast<int>(ActionTag::puffed));
  this->runAction(mt);
  this->runAction(CC::FadeOut::create(4.f));
}
