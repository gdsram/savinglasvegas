/*! \brief This file have the interface for Clouds class
    \file clouds.h
    \author Copyright owners
    \date 6/20/2014

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

#ifndef CLASSES_GAMEOBJECT_CLOUDS_H
#define CLASSES_GAMEOBJECT_CLOUDS_H

#include <random>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/appmacros.h"
#include "Classes/globalcolaboration.h"
#include "Classes/GameObject/cloud.h"

class Clouds : public CC::Ref {
 public:
  ~Clouds() = default;

  CREATE_FUNC(Clouds)
  inline bool init() noexcept {
    mvisible_size_ = CC_DIRECTOR_->getVisibleSize();
    for (std::uint8_t i{0}; i < cMaxCloudsNumber_; i++) {
        Cloud *cloud = Cloud::create();
        clouds_.pushBack(cloud);
    }
    return true;
  }
  inline void setToInitialState() noexcept {
    for (Cloud* cloud : clouds_) {
      cloud->setToInitialState();
    }
  }
  // initAtTheBeginingOfTheGame
  inline void pause() noexcept {
    for (Cloud *cloud : clouds_) {
      cloud->pause();
    }
  }
  inline void resume() noexcept {
    for (Cloud *cloud : clouds_) {
      cloud->resume();
    }
  }
  inline void stop() noexcept {
    for (Cloud *cloud : clouds_) {
      cloud->stop();
    }
  }
  const CC::Vector<Cloud*>& clouds() const noexcept {
    return clouds_;
  }
  // TODO(denisacostaq@gmail.com): en este tipo de funciones se puede tener una
  // mejora de la eficiencia sin recorrer el arreglo teniendo un contadro de las
  // visibles.
  inline void launchNext(const float& offsetX, const float &offsetY) noexcept {
    for (Cloud *cloud : clouds_) {
      if (!cloud->isVisible()) {
        cloud->launch(offsetX, offsetY);
        break;
      }
    }
  }
  // TODO: esto se puede optimizar si notifico cuando se incativan pa tra.
  inline const std::uint8_t visibleClouds() const noexcept {
    std::uint8_t ret{0};
    for (Cloud* cloud : clouds_) {
      if (cloud->isVisible() == true) {
        ++ret;
      }
    }
    return ret;
  }

 private:
  const std::uint8_t cMaxCloudsNumber_{15};
  CC::Vector<Cloud*> clouds_;
  CC::Size mvisible_size_;

  Clouds();

  Clouds(const Clouds&) = delete;
  Clouds& operator=(Clouds&) = delete;

  Clouds(const Clouds&&) = delete;
  Clouds& operator=(Clouds&&) = delete;
};

#endif  // CLASSES_GAMEOBJECT_CLOUDS_H
