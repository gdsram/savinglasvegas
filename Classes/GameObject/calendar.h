/*! \brief This file have the interface for Calendar class.
    \file calendar.h
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

#ifndef CLASSES_GAMEOBJECT_CALENDAR_H
#define CLASSES_GAMEOBJECT_CALENDAR_H

//  FIXME: no portable
#include <cinttypes>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/globalcolaboration.h"

class Calendar : public CC::Node {
 public:
  ~Calendar();

  Calendar(const Calendar&) = delete;
  Calendar& operator=(const Calendar&) = delete;

  Calendar(const Calendar&&) = delete;
  Calendar& operator=(const Calendar&&) = delete;

  CREATE_FUNC(Calendar)

  virtual bool init() override;
  void setToInitialState() noexcept;
  inline const CC::Sprite* sun() const noexcept {
    return sun_;
  }
  inline const CC::Sprite* moon() const noexcept {
    return moon_;
  }
  inline const CC::Sprite* stars() const noexcept {
    return stars_;
  }
  inline const CC::Sprite* passed_days_background() const noexcept {
    return passed_days_background_;
  }
  inline const CC::Sprite* sky() const noexcept {
    return sky_;
  }
  inline const CC::LayerColor* nigth() const noexcept {
    return nigth_;
  }
  inline const std::int32_t passed_days() const noexcept {
    return passed_days_;
  }
  inline void pause() noexcept {
    CC::ActionManager* actionManager{CC_DIRECTOR_->getActionManager()};
    for (CC::Node *target : targets_to_pause_resume_stop_) {
      actionManager->pauseTarget(target);
    }
  }
  inline void resume() noexcept {
    static CC::ActionManager* actionManager{CC_DIRECTOR_->getActionManager()};
    actionManager->resumeTargets(targets_to_pause_resume_stop_);
  }
  inline void stop() noexcept {
    for (CC::Node *target : targets_to_pause_resume_stop_) {
      target->stopAllActions();
    }
  }
  inline void setVisible(bool visible) noexcept {
    passed_days_label_->setVisible(visible);
    passed_days_background_->setVisible(visible);
  }
  inline void setScoreLabel(CC::Label *scoreLabel) noexcept {
    passed_days_background_->addChild(scoreLabel);
    scoreLabel->setAnchorPoint(CC::Vec2::ANCHOR_MIDDLE_LEFT);
    scoreLabel->setColor(CC::Color3B{255, 25, 92});
    scoreLabel->setPosition(
          CC::Vec2{passed_days_background_->getContentSize().width*.4f,
                   passed_days_background_->getContentSize().height*.53f});
  }
 private:
  Calendar();
  void createActions() noexcept;
  void initActions() noexcept;
  void sunDone() noexcept;
  void moonDone() noexcept;
  inline void updatePassedDaysLabel() noexcept {
    char str[10]{0};
    std::snprintf(str, sizeof(str), "%d", passed_days_);
    passed_days_label_->setString(std::string{str});
  }

  CC::Size visible_size_;
  std::int32_t passed_days_;
  CC::Label *passed_days_label_;
  CC::Sprite *passed_days_background_;
  std::int32_t day_duration_;
  std::int32_t nigth_duration_;
  CC::Sprite *sun_;
  CC::CardinalSplineTo *sun_path_;
  CC::Sprite *moon_;
  CC::CardinalSplineTo *moon_path_;
  CC::LayerColor *nigth_;
  CC::Sprite* stars_;
  CC::Sprite* sky_;
  CC::Vector<CC::Node*> targets_to_pause_resume_stop_;
  CC::Vec2 visible_origin_;
};

#endif  // CLASSES_GAMEOBJECT_CALENDAR_H
