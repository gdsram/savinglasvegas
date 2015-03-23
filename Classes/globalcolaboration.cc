/*! \brief This file have the implementation for GlobalColaboration class
    \file globalcolaboration.cc
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

#include "Classes/globalcolaboration.h"

GlobalColaboration* GlobalColaboration::self_instance_ {nullptr};
GlobalColaboration::GlobalColaboration()
  : bg_music_volume_{userDefaultData.bgMusicVolumeDefalutValue}
  , ef_music_volume_ {userDefaultData.efMusicVolumeDefalutValue}
  // TODO(denisacostaq\@gmail.com): segunda version.
  // , m_dificulty{userDefaultData.gameDificultyDefalutValue}
  // , m_currentLevel{userDefaultData.currentLevelDefalutValue}
  , audio_enabled_{userDefaultData.audioEnabledDefalutValue}
  , show_hints_{userDefaultData.showHintsDefaultValue}
  , height_score_{userDefaultData.heightScoreDefaultvalue}
  , paused_{false}
  , extra_width_factor_{.0f}
  , uniform_dist_whit_paralax_{
      std::uniform_real_distribution<float>{
        0,
        CC_DIRECTOR_->getVisibleSize().width * (.85f + extra_width_factor_)}} {

  // LOAD CONFIGURATION FROM PLIST
  loadBgMusicVolume();
  loadEfMusicVolume();
  // loadDificulty(); TODO(denisacostaq\@gmail.com): segunda version.
  // loadCurrentLevel(); TODO(denisacostaq\@gmail.com): segunda version.
  loadAudioEnabled();
  loadShowHints();
  loadHeightScore();
}

GlobalColaboration::~GlobalColaboration() {
  CC::UserDefault::getInstance()->destroyInstance();
}

// TODO(denisacostaq\@gmail.com): segunda version.
// void GlobalColaboration::loadDificulty() {
//       m_dificulty = CC::UserDefault::getInstance()->getFloatForKey(
//                   userDefaultData.gameDificulty,
//                   userDefaultData.gameDificultyDefalutValue);
// }

// void GlobalColaboration::loadCurrentLevel() {
//      m_currentLevel = CC::UserDefault::getInstance()->getIntegerForKey(
//                  userDefaultData.currentLevel,
//                  userDefaultData.currentLevelDefalutValue);
// }
