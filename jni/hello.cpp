#include "android_native_app_glue.h"
#include <android/log.h>
#include <jni.h>
void handle_activity_lifecycle_events(struct android_app *app, int32_t cmd) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "%d: dummy data %d",
                      cmd, *(reinterpret_cast<int *>(app->userData)));
}
void android_main(android_app *app) {
  app_dummy();
  {
    int dummy_data(111);
    app->userData = &dummy_data;
  }
  app->onAppCmd = handle_activity_lifecycle_events;
  while (true) {
    {
      int events;
      struct android_poll_source *source;
      int ident(ALooper_pollAll(-1, nullptr, &events,
                                reinterpret_cast<void **>(&source)));
      if ((0 <= ident)) {
        source->process(app, source);
      }
    }
  }
}