LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
    ../../Classes/appdelegate.cc \
    ../../Classes/globalcolaboration.cc \
    ../../Classes/admobhelper.cc \
    ../../Classes/GameObject/weaponmanager.cc \
    ../../Classes/GameObject/tornado.cc \
    ../../Classes/GameObject/tornados.cc \
    ../../Classes/GameObject/package.cc \
    ../../Classes/GameObject/genericweapon.cc \
    ../../Classes/GameObject/genericweapons.cc \
    ../../Classes/GameObject/bomb.cc \
    ../../Classes/GameObject/bombs.cc \
    ../../Classes/GameObject/healt.cc \
    ../../Classes/GameObject/asteroids.cc \
    ../../Classes/GameObject/asteroid.cc \
    ../../Classes/GameObject/calendar.cc \
    ../../Classes/GameObject/atomicbomb.cc \
    ../../Classes/GameObject/atomicbombs.cc \
    ../../Classes/GameObject/clouds.cc \
    ../../Classes/GameObject/cloud.cc \
    ../../Classes/GameObject/packages.cc \
    ../../Classes/Scene/splashscene.cc \
    ../../Classes/Scene/menuscene.cc \
    ../../Classes/Scene/gamelayer.cc

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
