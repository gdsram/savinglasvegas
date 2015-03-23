/*! \brief This file have the implementation for Calendar class.
    \file calendar.cc
    \author Copyright owners
    \date 5/16/2014

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

#include "Classes/GameObject/calendar.h"

Calendar::Calendar()
  : visible_size_{CC::Size::ZERO}
  , passed_days_{0}
  , passed_days_label_{nullptr}
  , passed_days_background_{nullptr}
  , day_duration_{45}
  , nigth_duration_{30}
  , sun_{nullptr}
  , sun_path_{nullptr}
  , moon_{nullptr}
  , moon_path_{nullptr}
  , nigth_{nullptr}
  , stars_{nullptr}
  , sky_{nullptr}
  , targets_to_pause_resume_stop_{}
  , visible_origin_{CC::Vec2::ZERO} {
}

Calendar::~Calendar() {
  CC_SAFE_RELEASE(sun_path_);
  CC_SAFE_RELEASE(moon_path_);
}

bool Calendar::init() {
  visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
  visible_size_ = CC_DIRECTOR_->getVisibleSize();
  bool ret{true};
  if (ret) {
    if ((sun_ = CC::Sprite::create("sun.png")) != nullptr) {
    } else {
      ret = false;
    }
  }

  if (ret) {
    if ((moon_ = CC::Sprite::create("moon.png")) != nullptr) {
    } else {
      ret = false;
    }
  }

  if (ret) {
    if ((nigth_ = CC::LayerColor::create(
           CC::Color4B(0, 0, 0, 0),
           visible_size_.width * 2,
           visible_size_.height)) !=
        nullptr) {
      // FIXME(denisacostaq@gmail.com): coco bug
      nigth_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE);
      nigth_->setPosition(
            CC::Vec2 {
              visible_size_.width*.5f - nigth_->getContentSize().width*.5f,
              visible_size_.height*.5f - nigth_->getContentSize().height*.5f} +
                visible_origin_);
    } else {
      ret = false;
    }
  }

  if (ret) {
    stars_ = CC::Sprite::create("stars.png");
    stars_->setAnchorPoint(CC::Vec2{.5f, -1.f});
    // BUG(denisacostaq@gmail.com): Arreglar esto: + visible_origin_.y,
    // creo que es -.
    stars_->setPosition(
          CC::Vec2{visible_size_.width*.5f,
                   -visible_size_.height});
    sky_ = CC::Sprite::create("skype.png");
    sky_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_BOTTOM);
    sky_->setPosition(
          CC::Vec2{visible_size_.width*.5f, 0.f});
  }

  if (ret) {
    targets_to_pause_resume_stop_.pushBack(sky_);
    targets_to_pause_resume_stop_.pushBack(stars_);
    targets_to_pause_resume_stop_.pushBack(sun_);
    targets_to_pause_resume_stop_.pushBack(moon_);
  }

  if (ret) {
    passed_days_background_ = CC::Sprite::create("score_background.png");
    passed_days_background_->setScaleX(
          CC_DIRECTOR_->getOpenGLView()->getFrameSize().width/
          CC_DIRECTOR_->getOpenGLView()->getDesignResolutionSize().width);
    passed_days_background_->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
    passed_days_background_->setPosition(
          CC::Vec2 {passed_days_background_->getContentSize().width*.1f,
                    visible_size_.height -
                      passed_days_background_->getContentSize().height*.5f} +
                      visible_origin_);
  }

  if (ret) {
    passed_days_label_ = CC::Label::createWithTTF(
          "0",
          "fonts/Dosis-Light.otf",
          visible_size_.height*.045f);
    passed_days_background_->addChild(passed_days_label_);
    passed_days_label_->setColor(CC::Color3B{76, 76, 255});
    passed_days_label_->setPosition(
          CC::Vec2{passed_days_background_->getContentSize().width*.19f,
                   passed_days_background_->getContentSize().height*.53f});
  }

  if (ret) {
    this->createActions();
  }

  return ret;
}

void Calendar::setToInitialState() noexcept {
  sun_->stopAllActions();
  sun_->setPosition(
        CC::Vec2{-visible_size_.width *.6f -
                   sun_->getContentSize().width*.5f,
                 visible_size_.height*.8f});
  moon_->stopAllActions();
  moon_->setPosition(
        CC::Vec2{-visible_size_.width * .6f -
                   moon_->getContentSize().width*.5f,
                 visible_size_.height*.8f});

  passed_days_ = 0;
  this->updatePassedDaysLabel();

  nigth_->stopAllActions();
  nigth_->setOpacity(0);
  sky_->setOpacity(255);
  stars_->setOpacity(0);

  this->initActions();
  this->setVisible(true);
}

void Calendar::createActions() noexcept {
  CC::PointArray *sun_points{CC::PointArray::create(5)};
  sun_points->addControlPoint(CC::Vec2{-visible_size_.width * .6f -
                                         sun_->getContentSize().width,
                                       visible_size_.height*.75f});
  sun_points->addControlPoint(CC::Vec2{0,
                                       visible_size_.height*.85f});
  sun_points->addControlPoint(CC::Vec2{visible_size_.width*.5f,
                                       visible_size_.height*.9f});
  sun_points->addControlPoint(CC::Vec2{visible_size_.width,
                                       visible_size_.height*.85f});
  sun_points->addControlPoint(CC::Vec2{visible_size_.width * 1.6f +
                                          sun_->getContentSize().width,
                                       visible_size_.height*.75f});
  sun_path_ = CC::CardinalSplineTo::create(day_duration_*.4f,
                                           sun_points,
                                           .0f);
  sun_path_->retain();
  CC::PointArray *moon_points{CC::PointArray::create(5)};
  moon_points->addControlPoint(CC::Vec2{-visible_size_.width * .6f -
                                          moon_->getContentSize().width*.5f,
                                        visible_size_.height*.8f});
  moon_points->addControlPoint(CC::Vec2{-visible_size_.width*.1f,
                                        visible_size_.height*.9f});
  moon_points->addControlPoint(CC::Vec2{visible_size_.width*.5f,
                                        visible_size_.height});
  moon_points->addControlPoint(CC::Vec2{visible_size_.width*1.1f,
                                        visible_size_.height*.9f});
  moon_points->addControlPoint(CC::Vec2{visible_size_.width * 1.6f +
                                          moon_->getContentSize().width*.5f,
                                        visible_size_.height*.8f});
  moon_path_ = CC::CardinalSplineTo::create(nigth_duration_*.4f,
                                            moon_points,
                                            .5f);
  moon_path_->retain();
}

void Calendar::initActions() noexcept {
  sun_->runAction(CC::Sequence::create(
                    CC::DelayTime::create(day_duration_*.1f),
                    sun_path_,
                    CC::DelayTime::create(day_duration_*.9f),
                    CC::CallFunc::create(CC_CALLBACK_0(Calendar::sunDone,
                                                       this)),
                    nullptr));
}

void Calendar::sunDone() noexcept {
  stars_->setRotation(-50.f);
  stars_->runAction(
        CC::RotateBy::create(
          nigth_duration_ * float {.5f + .4f + .9f} + day_duration_*.17f,
          100.f));
  stars_->runAction(CC::FadeIn::create(nigth_duration_*.25f));
  nigth_->runAction(CC::FadeTo::create(nigth_duration_, 200));
  moon_->runAction(CC::Sequence::create(
                     CC::DelayTime::create(nigth_duration_*.5f),
                     moon_path_,
                     CC::DelayTime::create(nigth_duration_*.9f),
                     CC::CallFunc::create(CC_CALLBACK_0(Calendar::moonDone,
                                                        this)),
                     nullptr));
}

void Calendar::moonDone() noexcept {
  stars_->runAction(CC::FadeOut::create(day_duration_*.17f));
  nigth_->runAction(CC::FadeTo::create(day_duration_*.2f, 0));
  ++passed_days_;
  this->updatePassedDaysLabel();
  sun_->runAction(CC::Sequence::create(
                    CC::DelayTime::create(day_duration_*.5f),
                    sun_path_,
                    CC::DelayTime::create(day_duration_*.9f),
                    CC::CallFunc::create(CC_CALLBACK_0(Calendar::sunDone,
                                                       this)),
                    nullptr));
}
