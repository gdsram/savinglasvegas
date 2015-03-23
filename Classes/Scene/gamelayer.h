/*! \brief This file have the interface for GameLayer class
    \file gamelayer.h
    \author Copyright owners
    \date 12/2/2014

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

#ifndef CLASSES_SCENE_GAMELAYER_H
#define CLASSES_SCENE_GAMELAYER_H

#include <cstring>
#include <vector>
#include <chrono>

#include <SimpleAudioEngine.h>

#include "Classes/globalcolaboration.h"
#include "Classes/admobhelper.h"
#include "Classes/GameObject/asteroid.h"
#include "Classes/GameObject/calendar.h"
#include "Classes/GameObject/asteroids.h"
#include "Classes/GameObject/bombs.h"
#include "Classes/GameObject/atomicbombs.h"
#include "Classes/GameObject/packages.h"
#include "Classes/GameObject/clouds.h"
#include "Classes/GameObject/tornados.h"
#include "Classes/GameObject/weaponmanager.h"
#include "Classes/GameObject/healt.h"

// TODO: #include "scene/Level1/L1GameOverScene.h"

class GameLayer : public CC::Layer {
 public:
  GameLayer(const GameLayer&) = delete;
  GameLayer& operator=(const GameLayer&) = delete;
  GameLayer(const GameLayer&&) = delete;
  GameLayer& operator=(const GameLayer&&) = delete;

  static CC::Scene* getInstance() noexcept;
  static void destroyInstance() noexcept;
  CREATE_FUNC(GameLayer)
  virtual bool init() override;

  virtual bool onTouchBegan(CC::Touch *touch, CC::Event *event) override;
  /*! Usado para ver en toda la pantalla desplasando con el touchs
   * \brief onTouchesMoved levanta una variable boleana que me dice si
   * \brief interpretar el touch como un move o como un begin.
   * \param touches
   * \param event
   */
  virtual void onTouchMoved(CC::Touch *touch, CC::Event *event) override;
  virtual void onTouchEnded(CC::Touch *touch, CC::Event *event) override;

// see declaration order googlestylecode.

 private:
  enum class currentGameState {
      state_0,
      state_1,
      state_2,
      state_3,
      state_4,
      state_5,
      state_6,
      state_7
  };
  enum class zOrderInParalax {
    zIndex_0 = 0,
    zIndex_1,
    zIndex_2,
    zIndex_3,
    zIndex_4,
    zIndex_5,
    zIndex_6
  };
  enum class WeaponSelectorTag  {
    Bomb,
    AtomicBomb,
    Tornado
  };

  static constexpr std::uint8_t _initialCountRltzt{3};
  static constexpr float _initialReverseSpeed{8.f};
  static CC::Scene *_selfInstance;
  const std::function<void(CC::Node*)> _weaponExplodedCallback;
  const std::function<void(int)> increaseScoreFunction;  // portable int

  GameLayer();
  ~GameLayer();
  inline static CC::Scene* scene() noexcept {
    CC::Scene *scene{CC::Scene::create()};
    scene->addChild(GameLayer::create());
    return scene;
  }

  void createMemebers() noexcept;
  void createMenu() noexcept;
  void createGameOverLayer() noexcept;
  void createWeaponSelector() noexcept;
  void createParalaxEfect() noexcept;
  void createPools() noexcept;
  void createSound() noexcept;

  virtual void onEnter() override;

  /*!
   * \brief setToInitialState
   * \todo el restart no desactiva el paralax si ya esta activo...
   * \todo
   */
  void setToInitialState() noexcept;

  void initGameSchedulers() noexcept;
#ifdef EVALUATION_VERSION
  void evaluationVersionScheduled(float dt);
#endif
  void automaticParalaxEfect(float dt) noexcept;
  void onParalaxActivated(float dt) noexcept;
  void updateExtraWidthFactor(float dt) noexcept;
  void updateAsteroidsLauncherInterval(float dt) noexcept;
  void updateCloudssLauncherInterval(float dt) noexcept;
  void updateCurrentBombs(float dt) noexcept;
  void updatePackagesLauncherInterval(float dt) noexcept;
  void updateReverseSpeedInterval(float dt) noexcept;
  void launchFirstPackageScheduled(float dt) noexcept;
  void launchFirstAsteroidScheduled(float dt) noexcept;
  void launchFirstCloudScheduled(float dt) noexcept;
  void updateLaunchPackage(float dt) noexcept;
  inline void updateLaunchAsteroid(float dt) noexcept {
    asteroids_->launchNext(reverse_speed_, deceased_performed_callback_);
  }
  inline void updateLaunchCloud(float dt) noexcept {
      clouds_->launchNext(-visible_size_.width*.5f, 0.f);
  }
  void bombCheckCollitions(Bomb* bomb_wave,
                           std::uint8_t *inOutNeutralizedMenaces,
                           std::uint8_t *inOutHelpersBrokens) noexcept;
  void atomicBombCheckCollitions(AtomicBomb *atomicBombWave) noexcept;
  void tornadoCheckCollitions(Tornado *tornado,
                              std::uint8_t *inOutNeutralizedMenaces,
                              std::uint8_t *inOutHelpersBrokens) noexcept;
  void chainReaction(GenericWeapon *wave, GenericWeapon *weapon) noexcept;
  void updateBombsCollitions(float dt) noexcept;
  void updateAtomicBomsCollitions(float dt) noexcept;
  void updateTornadosCollitions(float dt) noexcept;
  void deceasedPerformedCallback(Node* pSender) noexcept;
  void packageFalled(Package *package) noexcept;

  /*! acsteroidDone An asteroid end the his action.
   * \brief asteroidDone An asteroid end the his action.
   * \param asteroid The asteroid that has been end the action.
   * \brief Is importany the order of the check because when this is performed
   * \brief this action is excecutin, this is a sequence.
   */
  inline void asteroidDone(Asteroid *asteroid) noexcept {
    if (asteroid->getActionByTag(
         static_cast<int>(Asteroid::ActTag::explosion))) {
      this->asteroidCatched(asteroid);
    } else if (asteroid->getActionByTag(
                static_cast<int>(Asteroid::ActTag::moveTo))) {
      this->asteroidFalled(asteroid);
    }
  }
  inline void asteroidCatched(Asteroid *asteroid) noexcept {
    asteroid->setToInitialState();
  }
  void asteroidFalled(Asteroid *asteroid) noexcept;
  void weaponExplodedCallback(Node* pSender) noexcept;
  void asteroidsRelentization() noexcept;

  void menuPauseCallback() noexcept;
  void pauseGame() noexcept;
  void menuResumeCallback() noexcept;
  void resumeGame() noexcept;
  void menuRestartCallback() noexcept;
  void menuExitCallback() noexcept;
#ifdef EVALUATION_VERSION
  void menuDownloadCallback() noexcept;
#endif
  void stopGame() noexcept;
  void deleteSound() noexcept;
  inline void increaseScore(std::int32_t score) noexcept {
    score_ += score;
    score_label_->setString(std::to_string(score_));
  }


  CC::Sprite *asteroid_hint_;
  Asteroids *asteroids_;
  std::string bg_music_;
  Calendar *calendar_;
  CC::Sprite *cloud_hint_;
  Clouds *clouds_;
#ifdef EVALUATION_VERSION
  CC::Sprite *_ev_hint;
#endif
  CC::Repeat *dance0_;
  CC::Repeat *dance1_;
  CC::CallFuncN *deceased_performed_callback_;
  GlobalColaboration *gc_;
  CC::Sprite *game_over_sprite_;
  Healt *healt_;
  bool hints_actions_locket_;
  std::string lang_;
  CC::Menu *menu_;
  CC::Sprite *menu_items_background_;
  CC::Menu *menu_on_pause_;
  CC::Node *paralax_hint_;
  Node *paralax_middle_node_;
  CC::ParallaxNode *paralax_root_node_;
  CC::Sprite *packageid_hint_;
  Packages *packages_;
  WeaponManager *weapon_manager_;
  CC::Vec2 visible_origin_;
  CC::Size visible_size_;

  CC::Label *accurancy_label_;
  bool are_touching_;
  float asteroids_laucher_interval_;
  std::uint16_t brocken_asteroids_with_boms_;
  std::uint8_t count_rltzt_;
  std::uint8_t count_rltzt_acum_;
  float clouds_laucher_interval_;
  CC::Label *days_go_label_;
  std::uint32_t exploded_meteors_;
  CC::Sprite *gameover_info_;
  CC::Label *heigth_score_;
  float packages_laucher_interval_;
  bool paralax_is_active_;
  /*!
   * \brief m_meteorosDelay Temporizador para ir replanificando la tarea de
   * \brief lanzamiento de meteoritos de manera que al incrementar el tiempo
   * \brief esto ocurra mas seguido.
   */
  bool reschedule_meteors_;
  /*! \brief m_reverseSpeed of Meteors
   * \brief Mientras mas pequenno el valor mas rapido se moveran los meteoritos
   * \brief por tanto mas dificil, por eso se llama reverseSpeed.
   */
  float reverse_speed_;
  std::int32_t score_;
  CC::Label *score_label_;
  CC::Label *score_go_label_;
  CC::Label *total_score_label_;
  std::uint16_t used_bombs_;

//  CC::Label *_totalScoreLabel;
};
#endif  // CLASSES_SCENE_GAMELAYER_H
