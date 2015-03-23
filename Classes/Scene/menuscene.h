/*! \brief Do not include this file directly in external modules.
    \file menuscene.h
    \author Copyright owners
    \date 2/3/2014

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

#ifndef CLASSES_SCENE_MENUSCENE_H
#define CLASSES_SCENE_MENUSCENE_H

#include "Classes/Scene/gamelayer.h"

class MenuScene : public CC::Layer {
 public:
  static CC::Scene *scene();
  CREATE_FUNC(MenuScene)
  bool init() override;
  virtual void onEnter() override;

  MenuScene(const MenuScene&)=delete;
  MenuScene& operator=(const MenuScene&)=delete;

  MenuScene(const MenuScene&&)=delete;
  MenuScene& operator=(const MenuScene&&)=delete;

  ~MenuScene();

 private:
  MenuScene();

  inline void createSound() noexcept {
    if (G_COLABORATION_->audio_enabled()) {
      CC_AUDIO_->preloadBackgroundMusic("MainMenu/MainMenuMusic.ogg");
    }
  }
  inline void deleteSound() noexcept {
    if (G_COLABORATION_->audio_enabled()) {
      CC_AUDIO_->stopBackgroundMusic(true);
    }
  }
  void createMenu() noexcept;
  void createCredits() noexcept;

  void menuPlayCallback() noexcept;
  inline void showCreditsCallback() noexcept {
    G_COLABORATION_->playEffect("MainMenu/show_credits.ogg");
    menu_bg_->setVisible(false);
    credits_->setScale(.1f);
    credits_->setVisible(true);
    credits_->runAction(
          CC::EaseElasticOut::create(CC::ScaleTo::create(1.7f, 1.f)));
  }
  inline void showMenuCallback() noexcept {
    credits_->setVisible(false);
    menu_bg_->setScale(.01f);
    menu_bg_->setVisible(true);
    menu_bg_->runAction(CC::ScaleTo::create(.7f, 1.f));
  }
  void menuExitCallback() noexcept;
  inline void updateLaunchCloud(float dt) noexcept {
    clouds_->launchNext(-mvisible_size_.width*.05f, visible_origin_.y);
  }

  bool restart_music_;
  CC::Size mvisible_size_;
  CC::Vec2 visible_origin_;
  std::string bg_music_;
  CC::Label *heigth_score_;
  Clouds *clouds_;
  CC::Sprite *credits_;
  CC::Sprite *menu_bg_;
  CC::Sprite *bg_;
};
#endif  // CLASSES_SCENE_MENUSCENE_H
