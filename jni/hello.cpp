#include "android_native_app_glue.h"
#include <android/log.h>
void drawSomething(android_app *app) {
  {
    auto win(app->window);
    ANativeWindow_setBuffersGeometry(win, 0, 0, WINDOW_FORMAT_RGBA_8888);
    {
      ANativeWindow_Buffer buf;
      if ((ANativeWindow_lock(win, &buf, nullptr) < 0)) {
        return;
      }
      memset(buf.bits, 0xFF0A0AAA,
             (buf.stride * buf.height * sizeof(uint32_t)));
      {
        char *p(reinterpret_cast<char *>(buf.bits));
        for (unsigned int i = 0; (i < 256); i += 1) {
          for (unsigned int j = 0; (j < 256); j += 1) {
            p[(0 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] =
                static_cast<char>(255);
            p[(1 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 0;
            p[(2 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] = 0;
            p[(3 + (sizeof(uint32_t) * ((i * buf.stride) + j)))] =
                static_cast<char>(255);
          }
        }
      }
      ANativeWindow_unlockAndPost(win);
    }
  }
}
void handle_activity_lifecycle_events(struct android_app *app, int32_t cmd) {
  __android_log_print(ANDROID_LOG_INFO, "native-activity", "%d: dummy data %d",
                      cmd, *(reinterpret_cast<int *>(app->userData)));
  switch (cmd) {
  case APP_CMD_INIT_WINDOW: {
    drawSomething(app);
    break;
  }
  }
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