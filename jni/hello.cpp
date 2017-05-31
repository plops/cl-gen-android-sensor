#include <android/log.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/rect.h>
#include <android/window.h>
#include <jni.h>
#include <unistd.h>
AInputQueue *g_input_queue(nullptr);
ANativeWindow *g_window(nullptr);

static void onStart(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onStart %p",
                      activity);
}
static void onResume(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onResume %p",
                      activity);
}
static void *onSaveInstanceState(ANativeActivity *activity, size_t *outlen) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onSaveInstanceState %p", activity);
  return nullptr;
}
static void onPause(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onPause %p",
                      activity);
}
static void onStop(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onStop %p",
                      activity);
}
static void onDestroy(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onDestroy %p",
                      activity);
}
static void onWindowFocusChanged(ANativeActivity *activity, int focused) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onWindowFocusChanged %p", activity);
}
static void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onNativeWindowCreated %p", activity);
  g_window = w;
}
static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onNativeWindowResized %p", activity);
}
static void onNativeWindowRedrawNeeded(ANativeActivity *activity,
                                       ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onNativeWindowRedrawNeeded %p", activity);
}
static void onNativeWindowDestroyed(ANativeActivity *activity,
                                    ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onNativeWindowDestroyed %p", activity);
}
static void onInputQueueCreated(ANativeActivity *activity, AInputQueue *in) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onInputQueueCreated %p", activity);
}
static void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *in) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onInputQueueDestroyed %p", activity);
  g_input_queue = in;
}
static void onContentRectChanged(ANativeActivity *activity, const ARect *rect) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onContentRectChanged %p", activity);
}
static void onConfigurationChanged(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onConfigurationChanged %p", activity);
}
static void onLowMemory(ANativeActivity *activity) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onLowMemory %p",
                      activity);
}
static void *android_app_create(ANativeActivity *activity, void *savedState,
                                size_t savedStateSize) {
  while (true) {
    {
      static bool window_initialized_p(false);
      if (((false == window_initialized_p) && (nullptr != g_window))) {
        window_initialized_p = true;
      } else {
        if ((nullptr != g_window)) {
          __android_log_print(ANDROID_LOG_INFO, "native-activity", "draw");
          ANativeWindow_setBuffersGeometry(g_window, 0, 0,
                                           WINDOW_FORMAT_RGBA_8888);
          {
            ANativeWindow_Buffer buf;
            if ((0 <= ANativeWindow_lock(g_window, &buf, nullptr))) {
              memset(buf.bits, 0xFF0A0AAA,
                     (buf.stride * buf.height * sizeof(uint32_t)));
              {
                char *p(reinterpret_cast<char *>(buf.bits));
                for (unsigned int i = 0; (i < 256); i += 1) {
                  for (unsigned int j = 0; (j < 256); j += 1) {
                    p[(0 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 255;
                    p[(1 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 0;
                    p[(2 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 0;
                    p[(3 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 255;
                  }
                }
              }
              ANativeWindow_unlockAndPost(g_window);
            }
          }
        }
      }
    }
    if ((nullptr != g_input_queue)) {
      while ((1 == AInputQueue_hasEvents(g_input_queue))) {
        {
          AInputEvent *event;
          AInputQueue_getEvent(g_input_queue, &event);
          AInputQueue_finishEvent(g_input_queue, event, 1);
          __android_log_print(ANDROID_LOG_INFO, "native-activity", "event %p",
                              event);
        }
      }
    }
    usleep(1000);
  }
  return nullptr;
}
void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState,
                              size_t savedStateSize) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "%p", activity);
  activity->callbacks->onStart = onStart;
  activity->callbacks->onResume = onResume;
  activity->callbacks->onSaveInstanceState = onSaveInstanceState;
  activity->callbacks->onPause = onPause;
  activity->callbacks->onStop = onStop;
  activity->callbacks->onDestroy = onDestroy;
  activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
  activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
  activity->callbacks->onNativeWindowResized = onNativeWindowResized;
  activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
  activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
  activity->callbacks->onInputQueueCreated = onInputQueueCreated;
  activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
  activity->callbacks->onContentRectChanged = onContentRectChanged;
  activity->callbacks->onConfigurationChanged = onConfigurationChanged;
  activity->callbacks->onLowMemory = onLowMemory;
  activity->instance = android_app_create(activity, savedState, savedStateSize);
}