/*! \brief This file have the interface for GlobalColaboration class
    \file globalcolaboration.h
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

#ifndef CLASSES_GLOBALCOLABORATION_H
#define CLASSES_GLOBALCOLABORATION_H

#include <random>

#include "Classes/appmacros.h"

class GlobalColaboration {
 public:
  GlobalColaboration(const GlobalColaboration&) = delete;
  GlobalColaboration& operator=(const GlobalColaboration&) = delete;

  GlobalColaboration(const GlobalColaboration&&) = delete;
  GlobalColaboration& operator=(const GlobalColaboration&&) = delete;

  inline static GlobalColaboration *self_instance() noexcept {
    if (!self_instance_) {
      self_instance_ = new GlobalColaboration();
    }
    return self_instance_;
  }

  void generateTrayectory(float *outInitX,
                          float *outEndX) noexcept {
    static const CC::Size visibleSize{CC_DIRECTOR_->getVisibleSize()};
    std::random_device rd;
    std::default_random_engine e1{rd()};
    *outInitX = uniform_dist_whit_paralax_(e1);
    (*outInitX) -= (visibleSize.width * extra_width_factor_)/2;
    *outEndX = uniform_dist_whit_paralax_(e1);
    (*outEndX) -= (visibleSize.width * extra_width_factor_)/2;
  }

  inline const float& bg_music_volume() const noexcept {
    return bg_music_volume_;
  }
  inline const float& ef_music_volume() const noexcept {
    return ef_music_volume_;
  }
// TODO(denisacostaq\@gmail.com): segunda version.
//  inline const float& getDificulty() const {
//    return m_dificulty;
//  }
  inline const bool& audio_enabled() const noexcept {
    return audio_enabled_;
  }
  inline const bool& show_hints() const noexcept {
    return show_hints_;
  }
  inline const std::int32_t& height_score() const noexcept {
    return height_score_;
  }
// TODO(denisacostaq\@gmail.com): segunda version.
//  inline const int8_t& getCurrentLevel() {
//    return m_currentLevel;
//  }


//  inline void set_bg_music_volume(const float &val) {
//    CC::UserDefault::getInstance()->setFloatForKey(
//                userDefaultData.bgMusicVolume, (bg_music_volume_ = val));
//    CC::UserDefault::getInstance()->flush();
//  }

//  inline void set_ef_music_volume(const float &val) {
//    CC::UserDefault::getInstance()->setFloatForKey(
//                userDefaultData.efMusicVolume, (ef_music_volume_ = val));
//    CC::UserDefault::getInstance()->flush();
//  }

// TODO(denisacostaq\@gmail.com): segunda version.
//  inline void setDificulty(const float &val) {
//    if (CC::UserDefault::getInstance()->isXMLFileExist()) {
//      CC::UserDefault::getInstance()->setFloatForKey(
//                  userDefaultData.gameDificulty, (m_dificulty = val));
//    }
//  }

  void set_audio_enabled(const bool& val) {
    CC::UserDefault::getInstance()->setBoolForKey(
                userDefaultData.audioEnabled, (audio_enabled_ = val));
    CC::UserDefault::getInstance()->flush();
  }

//  inline void setCurrentLevel(const std::int8_t& val) {
//    if (CC::UserDefault::getInstance()->isXMLFileExist()) {
//      CC::UserDefault::getInstance()->setIntegerForKey(
//                  userDefaultData.currentLevel, (m_currentLevel = val));
//    }
//  }

  inline void set_show_hints(const bool& val) {
    CC::UserDefault::getInstance()->setBoolForKey(
          userDefaultData.showHints, (show_hints_ = val));
    CC::UserDefault::getInstance()->flush();
  }
  inline void set_height_score(const std::int32_t& val) {
    if (height_score() < val) {
      CC::UserDefault::getInstance()->setIntegerForKey(
            userDefaultData.heightScore, (height_score_ = val));
      CC::UserDefault::getInstance()->flush();
    }
  }

  inline void playEffect(std::string file) noexcept {
    if (audio_enabled_) {
      static CocosDenshion::SimpleAudioEngine *instance{CC_AUDIO_};
      instance->playEffect(file.c_str());
    }
  }

  bool paused() {
    return paused_;
  }
  void pause() {
    paused_ = true;
  }
  void resume() {
    paused_ = false;
  }

  inline const float& extra_width_factor() const noexcept {
    return extra_width_factor_;
  }
  inline void set_extra_width_factor(const float& extraWidthFactor) noexcept {
    extra_width_factor_ = extraWidthFactor;
    uniform_dist_whit_paralax_ =
        std::uniform_real_distribution<float>{
          0,
          CC_DIRECTOR_->getVisibleSize().width * (1 + extra_width_factor_)
        };
  }

  const std::string langueageSufix() const noexcept {
    std::string sufix{"_us"};
    switch (CC::Application::getInstance()->getCurrentLanguage()) {
      case CC::LanguageType::ENGLISH:
        sufix = "_us";
        break;
      case CC::LanguageType::SPANISH:
        sufix = "_es";
        break;
      default:
        sufix = "_us";
        break;
    }
    return sufix;
  }


 private:
  static GlobalColaboration* self_instance_;
  GlobalColaboration();

  /*!
   * \brief GlobalColaboration::~GlobalColaboration es privado, nunca elimine la
   * \brief instancia en una clase que la haya obtenido a travez de getInstance()
   * \sa getInstance ();
   */
  ~GlobalColaboration();

  float bg_music_volume_;
  float ef_music_volume_;
// float m_dificulty; TODO(denisacostaq\@gmail.com): segunda version.
// std::int8_t m_currentLevel;
  bool audio_enabled_;
  bool show_hints_;
  std::int32_t height_score_;

  bool paused_;
  float extra_width_factor_;
  std::uniform_real_distribution<float> uniform_dist_whit_paralax_;

  void loadBgMusicVolume() noexcept {
    bg_music_volume_ =  CC::UserDefault::getInstance()->getFloatForKey(
                userDefaultData.bgMusicVolume,
                userDefaultData.bgMusicVolumeDefalutValue);
  }

  void loadEfMusicVolume() noexcept {
      ef_music_volume_ = CC::UserDefault::getInstance()->getFloatForKey(
                  userDefaultData.efMusicVolume,
                  userDefaultData.efMusicVolumeDefalutValue);
  }
  // void loadDificulty(); TODO(denisacostaq\@gmail.com): segunda version.
  // void loadCurrentLevel();
  void loadAudioEnabled() {
    audio_enabled_ = CC::UserDefault::getInstance()->getBoolForKey(
                userDefaultData.audioEnabled,
                userDefaultData.audioEnabledDefalutValue);
  }

  void loadShowHints() {
    show_hints_ = CC::UserDefault::getInstance()->getBoolForKey(
          userDefaultData.showHints,
          userDefaultData.showHintsDefaultValue);
  }

  void loadHeightScore() {
    height_score_ = CC::UserDefault::getInstance()->getIntegerForKey(
          userDefaultData.heightScore,
          userDefaultData.heightScoreDefaultvalue);
  }
};

// #ifndef GLOBALCOLABORATIONNP
// #define GLOBALCOLABORATIONNP
// namespace GlobalColaborationNP {
// #ifdef EVALUATION_VERSION
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//   #include <platform/android/jni/JniHelper.h>
//   extern bool openURL(const char* url) {
//       cocos2d::JniMethodInfo t;
//       const char * CLASS_NAME = "org/cocos2dx/lib/Cocos2dxHelper";

//       bool ret = false;
//       if (CC::JniHelper::getStaticMethodInfo(t,
//                                              CLASS_NAME,
//                                              "openURL",
//                                              "(Ljava/lang/String;)Z")) {
//           jstring stringArg = t.env->NewStringUTF(url);
//           ret = t.env->CallStaticBooleanMethod(
                        // t.classID, t.methodID, stringArg);

//           t.env->DeleteLocalRef(t.classID);
//           t.env->DeleteLocalRef(stringArg);
//       }
//       return ret;
//   }
// #else
//   extern bool openURL(const char* url) { return true;}
// #endif  // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
// #endif  // EVALUATION_VERSION
// }
// #endif  // GLOBALCOLABORATIONNP

#endif  // CLASSES_GLOBALCOLABORATION_H
