LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_CFLAGS := -std=gnu++11
#LOCAL_CPPFLAGS += -Wall -Wshadow -Wextra -pedantic -std=c++11 -frtti
LOCAL_CPPFLAGS +=  -std=c++11 -frtti
LOCAL_CPPFLAGS += -fexceptions -I../include
DEPENDENCIES_PATH:=$(LOCAL_PATH)/../
SDL_PATH := $(DEPENDENCIES_PATH)/SDL/
SDL_IMAGE_PATH :=$(DEPENDENCIES_PATH)/SDL2_image/
SDL_TTF_PATH :=$(DEPENDENCIES_PATH)/SDL2_ttf/
SPDLOG :=$(DEPENDENCIES_PATH)/spdlog/include/

LOCAL_C_INCLUDES := $(SDL_PATH)/include $(SDL_IMAGE_PATH)/ $(SDL_TTF_PATH) $(SPDLOG)

# Add your application source files here...
LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp) $(wildcard $(LOCAL_PATH)/engine/*.cpp) $(wildcard $(LOCAL_PATH)/engine/impl/*.cpp) $(wildcard $(LOCAL_PATH)/engine/drawcmd/*.cpp) $(wildcard $(LOCAL_PATH)/engine/events/*.cpp)   \
		$(wildcard $(LOCAL_PATH)/game/*.cpp) $(wildcard $(LOCAL_PATH)/game/logic/*.cpp) $(wildcard $(LOCAL_PATH)/game/stage/*.cpp) $(wildcard $(LOCAL_PATH)/game/su40/*.cpp)
#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/engine/*.cpp)
#LOCAL_SRC_FILES := $(wildcard $(LOCAL_PATH)/engine/controllers/*.cpp)

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
