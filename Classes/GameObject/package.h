/*! \brief This file have the interface for Package class
    \file package.h
    \author Copyright owners
    \date 6/12/2014

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
#ifndef CLASSES_GAMEOBJECT_PACKAGE_H
#define CLASSES_GAMEOBJECT_PACKAGE_H

#include <functional>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/appmacros.h"
#include "Classes/globalcolaboration.h"

class Package : public CC::Sprite {
 public:
  enum class Kind {
    Bomb,
    AtomicBomb,
    Tornado,
    Healt
  };

  enum class ActionTag {
    falling,
    swing,
    puffed
  };

  ~Package() = default;

  Package(const Package&) = delete;
  Package& operator=(const Package&) = delete;

  Package(const Package&&) = delete;
  Package& operator=(const Package&&) = delete;

  CREATE_FUNC(Package)
  bool init() override;
  inline virtual void onEnter() override {
    CC::Sprite::onEnter();
    this->setToInitialState();
  }
  inline void pause() noexcept {
    CC_DIRECTOR_->getActionManager()->pauseTarget(this);
    CC_DIRECTOR_->getEventDispatcher()->pauseEventListenersForTarget(this);
  }
  inline void resume() noexcept {
    CC_DIRECTOR_->getActionManager()->resumeTarget(this);
    CC_DIRECTOR_->getEventDispatcher()->resumeEventListenersForTarget(this);
  }
  void setToInitialState() noexcept;

  void setKind(const Kind& kind) noexcept;
  const inline Kind& getKind() const noexcept {
    return kind_;
  }
  void launch(float reverseSpeed) noexcept;
  void puff(CC::Vec2 from) noexcept;

 private:
  Package() = default;
  Kind kind_;
  CC::Vec2 visible_origin_;
};

#endif  // CLASSES_GAMEOBJECT_PACKAGE_H
