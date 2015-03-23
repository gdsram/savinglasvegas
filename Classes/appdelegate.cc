/*! \brief This file have the implementation for AppDelegate class.
    \file appdelegate.cc
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

#include "Classes/appdelegate.h"

bool AppDelegate::applicationDidFinishLaunching() {
  CC::Configuration::getInstance()->loadConfigFile(
        "configs/config-example.plist");
  CC::GLView* pEGLView{CC_DIRECTOR_->getOpenGLView()};
  if (!pEGLView) {
    pEGLView = CC::GLView::create("Saving Las Vegas");
    CC_DIRECTOR_->setOpenGLView(pEGLView);
  }
//  pEGLView->setFrameSize(1024.f, 500.f);
  CC::Size designResolutionSize{960.f, 640.f};
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//     // a bug in DirectX 11 level9-x on the device prevents
       // ResolutionPolicy::NO_BORDER from working correctly
//   pEGLView->setDesignResolutionSize(designResolutionSize.width,
//                                     designResolutionSize.height,
//                                     ResolutionPolicy::SHOW_ALL);
// #else
//   pEGLView->setDesignResolutionSize(designResolutionSize.width,
//                                     designResolutionSize.height,
//                                     ResolutionPolicy::NO_BORDER);
// #endif
//   CC::log("f w %f", pEGLView->getFrameSize().width);
//   CC::log("f h %f", pEGLView->getFrameSize().height);
//   CC::log("d w %f", pEGLView->getDesignResolutionSize().width);
//   CC::log("d h %f", pEGLView->getDesignResolutionSize().height);
//   CC::log("o x %f", CC_DIRECTOR_->getVisibleOrigin().x);
//   CC::log("o y %f", CC_DIRECTOR_->getVisibleOrigin().y);

  Resource res{std::move(mediumHorizontalResource)};
  CC_DIRECTOR_->setContentScaleFactor(
        std::min(res.size_.height/designResolutionSize.height,
                 res.size_.width/designResolutionSize.width));

  CC::FileUtils::getInstance()->setSearchPaths(
        std::vector<std::string>{res.directory_, "Sounds/"});

  // scene: it's an autorelease object
  CC_DIRECTOR_->runWithScene(SplashScene::scene());
  return true;
}

// TODO
void AppDelegate::applicationDidEnterBackground() {
  CC_DIRECTOR_->stopAnimation();
  CC_AUDIO_->pauseBackgroundMusic();
}

// TODO
void AppDelegate::applicationWillEnterForeground() {
  CC_DIRECTOR_->startAnimation();
  CC_AUDIO_->resumeBackgroundMusic();
}
