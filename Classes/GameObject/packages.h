/*! \brief This file have the interface for Packages class
    \file packages.h
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
#ifndef CLASSES_GAMEOBJECT_PACKAGES_H
#define CLASSES_GAMEOBJECT_PACKAGES_H

#include <functional>

#include <cocos2d.h>
namespace CC = cocos2d;

#include "Classes/GameObject/package.h"

class Packages : public CC::Ref {
 public:
  ~Packages() = default;
  Packages(const Packages&) = delete;
  Packages& operator=(const Packages&) = delete;
  Packages(const Packages&&) = delete;
  Packages& operator=(const Packages&&) = delete;

  static Packages *create(
    const std::function<void(CC::Node*)> &takenCallback) noexcept;
  bool init();
  void setToInitialState() noexcept;
  void launchNext(Package::Kind package_kind, float reverseSpeed) noexcept;
  inline CC::Vector<Package*> packages() const noexcept {
    return packages_;
  }
  void setPendantPackages(Package::Kind package, std::uint8_t pendant) noexcept;
  const std::uint8_t pendantPackages(Package::Kind package) const noexcept;

  inline void pause() noexcept {
    for (Package *pk : packages_) {
      pk->pause();
    }
  }
  inline void resume() noexcept {
    for (Package *pk : packages_) {
      pk->resume();
    }
  }
  inline void stop() noexcept {
    for (Package *pk : packages_) {
      pk->stopAllActions();
    }
  }

 private:
  Packages(const std::function<void(CC::Node*)> &takenCallback);

  void initListener(Package *package) noexcept;

  CC::Vector<Package*> packages_;
  const std::int8_t m_cMaxPackagesNumber{3};
  std::int8_t packages_index_;
  std::uint8_t pendant_packages_bomb_;
  std::uint8_t pendant_packages_atomic_bomb_;
  std::uint8_t pendant_packages_tonado_;
  std::uint8_t pendant_packages_healt_;
  std::function<void(CC::Node*)> taken_callback_;
};

#endif  // CLASSES_GAMEOBJECT_PACKAGES_H
