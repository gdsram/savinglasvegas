/*! \brief This file have the interface for Asteroids class
    \file asteroids.h
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

#ifndef CLASSES_GAMEOBJECT_ASTEROIDS_H
#define CLASSES_GAMEOBJECT_ASTEROIDS_H

#include <cstdint>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/GameObject/asteroid.h"

class Asteroids : public CC::Ref {
 public:

  ~Asteroids() = default;

  Asteroids(const Asteroids&) = delete;
  Asteroids& operator=(const Asteroids&) = delete;

  Asteroids(const Asteroids&&) = delete;
  Asteroids& operator=(const Asteroids&&) = delete;

  CREATE_FUNC(Asteroids)

  inline void setToInitialState() noexcept {
    for (Asteroid *meteor : meteors_) {
      meteor->setToInitialState();
    }
  }
  inline const CC::Vector<Asteroid*>& meteors() const noexcept {
    return meteors_;
  }
  inline const CC::Vector<CC::Label*>& nums() noexcept {
    return numss_;
  }
  inline void launchNext(float reverseSpeed, CC::CallFuncN *callback) noexcept {
    if (meteors_index_ == cMaxMeteorsNumber_) {
      meteors_index_ = 0;
    }
    //  FIXME(denisacostaq): verificar que este disponible
    meteors_.at(meteors_index_++)->launch(reverseSpeed, callback);
  }
  inline void pause() const noexcept {
    for (Asteroid *it : meteors_) {
      it->pause();
    }
  }
  inline void resume() const noexcept {
    for (Asteroid *it : meteors_) {
      it->resume();
    }
  }
  inline void stop() const noexcept {
    for (Asteroid *it : meteors_) {
      it->stop();
    }
  }

 private:
  CC::Size mvisible_size_;
  std::int8_t meteors_index_;
  CC::Vector<Asteroid*> meteors_;
  CC::Vector<CC::Label*> numss_;
  static constexpr int8_t cMaxMeteorsNumber_{10u};
  CC::Vec2 visible_origin_;

  Asteroids();
  inline bool init() noexcept {
    visible_origin_ = CC_DIRECTOR_->getVisibleOrigin();
    initMeteors();
    initAnimations();
    return true;
  }
  void initMeteors() noexcept;
  void initAnimations() noexcept;
};

#endif  // CLASSES_GAMEOBJECT_ASTEROIDS_H
