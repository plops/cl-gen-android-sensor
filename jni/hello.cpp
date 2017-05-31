#include <android/log.h>
#include <android/native_activity.h>
#include <jni.h>
#include <string.h>
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
}
static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity",
                      "onNativeWindowResized %p", activity);
}
static void onNativeWindowRedrawNeeded(ANativeActivity *activity,
                                       ANativeWindow *w) {
  __android_log_print(ANDROID_LOG_INFO, "native-athe sectionctivity",
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
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "onLowMmemory %p",
                      activity);
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
  activity->instance = nullptr;
}