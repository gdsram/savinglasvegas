/*! \brief This file have the interface for Healt class.
    \file healt.h
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

#ifndef CLASSES_GAMEOBJECT_HEALT_H
#define CLASSES_GAMEOBJECT_HEALT_H

#include <cocos2d.h>
namespace  CC = cocos2d;

#include "Classes/appmacros.h"

class Healt : public CC::Sprite {
 public:
  ~Healt() = default;

  Healt(const Healt&) = delete;
  Healt& operator=(const Healt&) = delete;

  Healt(const Healt&&) = delete;
  Healt& operator=(const Healt&&) = delete;

  CREATE_FUNC(Healt)
  virtual bool init() override;
  inline void setToInitialState() noexcept {
    this->set_healt(100);
    this->updatePrograsBar();
    this->setVisible(true);
    healt_background_->setVisible(true);
  }

  inline const std::uint8_t& healt() const noexcept {
    return healt_;
  }
  void set_healt(const std::uint8_t& healt) noexcept;
  inline void set_healt_background(CC::Sprite *hb) noexcept {
    healt_background_ = hb;
    float xScale{
      CC_DIRECTOR_->getOpenGLView()->getFrameSize().width/
        CC_DIRECTOR_->getOpenGLView()->getDesignResolutionSize().width};
    healt_background_->setScaleX(xScale);
    this->setScaleX(xScale);
  }
  inline CC::Sprite* healt_background() const noexcept {
    return healt_background_;
  }
  inline void setVisible(bool visible) noexcept {
    CC::Sprite::setVisible(visible);
  }
  inline void pause() noexcept {
    CC::Node::pause();
  }
  inline void resume() noexcept {
    CC::Node::resume();
  }
  inline void stop() noexcept {
    this->stopAllActions();
  }

 private:
  std::uint8_t healt_;
  CC::Sprite *healt_background_;

  Healt();
  void updatePrograsBar() noexcept;

};

#endif  // CLASSES_GAMEOBJECT_HEALT_H
