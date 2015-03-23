/*! \brief This file have the interface for Asteroid class.
    \file asteroid.h
    \author Copyright owners
    \date 12/8/2014

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

#ifndef CLASSES_GAMEOBJECT_ASTEROID_H
#define CLASSES_GAMEOBJECT_ASTEROID_H

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/globalcolaboration.h"

/*! The L1Asteroid class is currently unused
 * \brief Esta clase se va a usar en caso que el crear y elminar
 * \brief Actions(MoveTo) tenga problemas de memoria ya que no
 * \brief se le puede reasignar el delta.
 */


class Asteroid : public CC::Sprite {
 public:
  enum class ActTag{moveTo, impact, explosion, rotation};
  ~Asteroid();


  Asteroid(const Asteroid&) = delete;
  Asteroid& operator=(const Asteroid&) = delete;

  Asteroid(const Asteroid&&) = delete;
  Asteroid& operator=(const Asteroid&&) = delete;

  static Asteroid* create(const std::string &filename) noexcept;
  virtual bool initWithFile(const std::string& filename) noexcept override;
  void setToInitialState() noexcept;

  inline void setDestiny(const CC::Vec2 &destiny) noexcept {
    destiny_ = destiny;
  }
  inline const CC::Vec2 destiny() const noexcept {
    return destiny_;
  }
  void launch(float reverseSpeed,
              CC::CallFuncN *callback) noexcept;

  void destroy(CC::CallFuncN *callback) noexcept;
  void impact() noexcept;

  inline void pause() noexcept {
    CC_DIRECTOR_->getActionManager()->pauseTarget(this);
    CC_DIRECTOR_->getActionManager()->pauseTarget(this->num_);
  }
  inline void resume() noexcept {
    CC_DIRECTOR_->getActionManager()->resumeTarget(this->num_);
    CC_DIRECTOR_->getActionManager()->resumeTarget(this);
  }
  inline void stop() noexcept {
    this->setVisible(false);
    this->stopAllActions();
    this->num_->stopAllActions();
  }
  const CC::Label *num() const noexcept {
    return num_;
  }

 private:
  CC::Vec2 destiny_;
  CC::Size visible_size_;
  CC::Label *num_;
  CC::Action *act_rot_;
  CC::Vec2 visible_origin_;
  Asteroid();

  inline void createAnimations() noexcept {
    act_rot_ = CC::RepeatForever::create(CC::RotateBy::create(3.f, 180.f));
    act_rot_->retain();
    act_rot_->setTag(static_cast<int>(ActTag::rotation));
  }
};
#endif  // CLASSES_GAMEOBJECT_ASTEROID_H
