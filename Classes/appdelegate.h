/*! \brief This file have the interface for AppDelegate class.
    \file appdelegate.h
    \author Copyright owners
    \date 10/24/2014

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

#ifndef CLASSES_APPDELEGATE_H
#define CLASSES_APPDELEGATE_H

#include <string>

#include "Classes/Scene/menuscene.h"
#include "Classes/Scene/splashscene.h"
#include "Classes/appmacros.h"

class AppDelegate : public CC::CCApplication {
 public:
  AppDelegate() = default;
  ~AppDelegate() = default;

  AppDelegate(const AppDelegate&) = delete;
  AppDelegate& operator=(const AppDelegate&) = delete;

  AppDelegate(const AppDelegate&&) = delete;
  AppDelegate& operator=(const AppDelegate&&) = delete;

  /*! Implement Director and Scene init code here.
\brief Here is the place for you.
  @return true    Initialize success, app continue.
  @return false   Initialize failed, app terminate.
  */
  virtual bool applicationDidFinishLaunching();

  /*! The function be called when the application enter background
  @param  the pointer of the application
  */
  virtual void applicationDidEnterBackground();

  /*! The function be called when the application enter foreground
@param  the pointer of the application
  */
  virtual void applicationWillEnterForeground();

 private:
};

#endif  // CLASSES_SCENE_APPDELEGATE_H
