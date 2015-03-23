/*! \brief This file have global utilitis
    \file AppMacros.h
    \author Alvaro Denis Acosta Quesada <denisacostaq\@gmail.com>
    \date Thu Oct 24 23:18:00 CDT 2013

    \brief This file become from: src/portable/AppMacros.h

    \attention <h1><center>&copy; COPYRIGHT
    GNU GENERAL PUBLIC LICENSE Version 2, June 1991</center></h1>

    \copyright

    <h3>This file is part of <a href="http://www.skypedefense.com"><strong>SkypeDenfense</strong></a> program, a small 2D game.</h3>
    Copyright (C) 2013  Alvaro Denis Acosta Quesada mailto:denisacostaq\@gmail.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef CLASSES_APPMACROS_H
#define CLASSES_APPMACROS_H

#include <cocos2d.h>
namespace CC = cocos2d;

// #define EVALUATION_VERSION ok

// TODO(denisacostaq@gmail.com): Use move contructor for copy and assing.
struct Resource {
  Resource(const Resource& resource) = delete;
  Resource& operator=(const Resource& resource) = delete;
  Resource()
    : size_{CC::Size::ZERO}
    , directory_{""} {
  }
  Resource(const cocos2d::Size& size, const std::string& directory)
    : size_{size}
    , directory_{directory} {
  }
  Resource(Resource&& resource)
    : size_{std::move(resource.size_)}
    , directory_{std::move(resource.directory_)} {
  }
  Resource& operator=(Resource&& resource) {
    size_ = std::move(resource.size_);
    directory_ = std::move(resource.directory_);
    return *this;
  }
  cocos2d::Size size_;
  std::string directory_;
};
using Resource = Resource;

struct UserDefaultData {
  const char *bgMusicVolume{"bgMusicVolume"};
  const float bgMusicVolumeDefalutValue {0.7f};
  const char *efMusicVolume{"efMusicVolume"};
  const float efMusicVolumeDefalutValue {0.9f};
//  const char *gameDificulty{"gameDificulty"};
//  const float gameDificultyDefalutValue{0.5f};
  const char *audioEnabled{"audioEnabled"};
  const bool audioEnabledDefalutValue{true};
//  const char *currentLevel{"currentLevel"};
//  const int8_t currentLevelDefalutValue{1};
  const char *showHints{"showHints"};
  bool showHintsDefaultValue{true};
  const char* heightScore{"heightScore"};
  const std::int32_t heightScoreDefaultvalue{0};
};
using UserDefaultData = UserDefaultData;

enum class zOrder {
  zIndex_0 = 0,
  zIndex_1,
  zIndex_2,
  zIndex_3,
  zIndex_4,
  zIndex_5,
  zIndex_6
};

enum class NodeTag{paralax, meteor, packet};

const UserDefaultData userDefaultData{};
// TODO(denisacostaq@gmail.com): Definir bien las resoluciones.
static Resource smallHorizontalResource {CC::Size{480,  320}, "IMG/480x320/"};
static Resource mediumHorizontalResource{CC::Size{960,  640}, "IMG/960x640/"};
static Resource heigthHorizontalResource{CC::Size{1920, 1280},
                                         "IMG/1920x1280/"};


#define CC_DIRECTOR_ cocos2d::Director::getInstance()
#define CC_AUDIO_ CocosDenshion::SimpleAudioEngine::getInstance()
#define G_COLABORATION_ GlobalColaboration::self_instance()
// #define CC_SFC_ cocos2d::SpriteFrameCache::getInstance()

#endif  // CLASSES_APPMACROS_H
