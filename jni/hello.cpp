#include "android_native_app_glue.h"
#include <algorithm>
#include <android/log.h>
#include <android/sensor.h>
#include <cmath>
enum Constants { M_MAG_N = 1200 };

int m_previous_x(-1);
float m_mag[M_MAG_N] = {
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f)};
float m_mag2[M_MAG_N] = {
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f), (0.0e+0f),
    (0.0e+0f), (0.0e+0f), (0.0e+0f)};
int m_mag_idx = 0;

struct userdata_t {
  int32_t move_x;
  ASensorEventQueue *sensor_event_queue;
  const ASensor *sensor_accelerometer;
};

int32_t handle_input_events(android_app *app, AInputEvent *event) {
  {
    auto etype(AInputEvent_getType(event));
    switch (etype) {
    case AINPUT_EVENT_TYPE_MOTION: {
      {
        auto action(AMotionEvent_getAction(event));
        int32_t pointer_index(
            ((action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
             AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT));
        int32_t pos_x(AMotionEvent_getY(event, pointer_index));
        if ((AMOTION_EVENT_ACTION_MOVE == action)) {
          {
            auto move_x((pos_x - m_previous_x));
            userdata_t *data(reinterpret_cast<userdata_t *>(app->userData));
            __android_log_print(ANDROID_LOG_INFO, "native-activity",
                                "motion %d", move_x);
            data->move_x = move_x;
            app->redrawNeeded = 1;
          }
        }
        m_previous_x = pos_x;
      }
      return 1;
      break;
    }
    }
  }
  return 1;
}
void inline set_pixel(ANativeWindow_Buffer &buf, int x, int y, int r, int g,
                      int b, int a) {
  {
    char *p(reinterpret_cast<char *>(buf.bits));
    auto pos((sizeof(uint32_t) * ((y * buf.stride) + x)));
    p[(0 + pos)] = static_cast<char>(r);
    p[(1 + pos)] = static_cast<char>(g);
    p[(2 + pos)] = static_cast<char>(b);
    p[(3 + pos)] = static_cast<char>(a);
  }
}
void draw_line(ANativeWindow_Buffer &buf, float x1, float y1, float x2,
               float y2) {
  {
    auto steep((std::abs((x2 - x1)) < std::abs((y2 - y1))));
    if (steep) {
      std::swap(x1, y1);
      std::swap(x2, y2);
    }
    if ((x2 < x1)) {
      std::swap(x1, x2);
      std::swap(y1, y2);
    }
    {
      auto dx((x2 - x1));
      auto dy(std::abs((y2 - y1)));
      auto err((dx / (2.e+0f)));
      const int ystep(((y1 < y2)) ? (1) : (-1));
      int y(static_cast<int>(y1));
      const int maxX(static_cast<int>(x2));
      for (int x = static_cast<int>(x1); (x < maxX); x += 1) {
        if (steep) {
          set_pixel(buf, y, x, 255, 0, 0, 255);
        } else {
          set_pixel(buf, x, y, 255, 0, 0, 255);
        }
        err -= dy;
        if ((err < 0)) {
          y += ystep;
          err += dx;
        }
      }
    }
  }
}
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
        static int x(0);
        x += reinterpret_cast<userdata_t *>(app->userData)->move_x;
        {
          auto mi(m_mag2[0]);
          auto ma(m_mag2[0]);
          for (unsigned int i = 0; (i < M_MAG_N); i += 1) {
            mi = std::min(m_mag2[i], mi);
            ma = std::max(m_mag2[i], ma);
          }
          for (unsigned int y = 0; (y < (M_MAG_N - 1)); y += 1) {
            {
              int xx1(static_cast<int>(
                  ((2.e-1f) +
                   (((6.e-1f) * (buf.stride - 1) * (m_mag2[(y + 1)] - mi)) /
                    (ma - mi)))));
              int xx0(static_cast<int>(
                  ((2.e-1f) +
                   (((6.e-1f) * (buf.stride - 1) * (m_mag2[(y + 0)] - mi)) /
                    (ma - mi)))));
              auto yy1((sizeof(uint32_t) * (((y + 1) + x) % buf.height)));
              auto yy0((sizeof(uint32_t) * (((y + 0) + x) % buf.height)));
              draw_line(buf, xx0, (yy0 / 4), xx1, (yy1 / 4));
            }
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
  case APP_CMD_GAINED_FOCUS: {
    {
      auto data(reinterpret_cast<userdata_t *>(app->userData));
      if ((nullptr != data->sensor_accelerometer)) {
        ASensorEventQueue_enableSensor(data->sensor_event_queue,
                                       data->sensor_accelerometer);
        ASensorEventQueue_setEventRate(data->sensor_event_queue,
                                       data->sensor_accelerometer,
                                       ((1000L / 5) * 1000));
      }
    }
    break;
  }
  case APP_CMD_LOST_FOCUS: {
    {
      auto data(reinterpret_cast<userdata_t *>(app->userData));
      if ((nullptr != data->sensor_accelerometer)) {
        ASensorEventQueue_disableSensor(data->sensor_event_queue,
                                        data->sensor_accelerometer);
      }
    }
    break;
  }
  }
}
void android_main(android_app *app) {
  app_dummy();
  {
    userdata_t data({0});
    auto sensor_manager(ASensorManager_getInstance());
    auto looper(ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS));
    if ((!((nullptr != looper)))) {
      __android_log_print(ANDROID_LOG_INFO, "native-activity",
                          "assertion (!= nullptr looper) failed");
    }
    if ((!((nullptr != sensor_manager)))) {
      __android_log_print(ANDROID_LOG_INFO, "native-activity",
                          "assertion (!= nullptr sensor_manager) failed");
    }
    data.sensor_accelerometer = ASensorManager_getDefaultSensor(
        sensor_manager, ASENSOR_TYPE_ACCELEROMETER);
    if ((!((nullptr != data.sensor_accelerometer)))) {
      __android_log_print(
          ANDROID_LOG_INFO, "native-activity",
          "assertion (!= nullptr data.sensor_accelerometer) failed");
    }
    data.sensor_accelerometer = ASensorManager_getDefaultSensor(
        sensor_manager, ASENSOR_TYPE_ACCELEROMETER);
    if ((!((nullptr != data.sensor_accelerometer)))) {
      __android_log_print(
          ANDROID_LOG_INFO, "native-activity",
          "assertion (!= nullptr data.sensor_accelerometer) failed");
    }
    data.sensor_event_queue = ASensorManager_createEventQueue(
        sensor_manager, looper, LOOPER_ID_USER, nullptr, nullptr);
    if ((!((nullptr != data.sensor_event_queue)))) {
      __android_log_print(
          ANDROID_LOG_INFO, "native-activity",
          "assertion (!= nullptr data.sensor_event_queue) failed");
    }
    app->userData = &data;
    app->onAppCmd = handle_activity_lifecycle_events;
    app->onInputEvent = handle_input_events;
    while ((0 == app->destroyRequested)) {
      {
        int events;
        android_poll_source *source;
        int ident(ALooper_pollAll((app->redrawNeeded) ? (0) : (-1), nullptr,
                                  &events, reinterpret_cast<void **>(&source)));
        if ((0 <= ident)) {
          switch (ident) {
          case LOOPER_ID_USER: {
            {
              ASensorEvent event;
              while ((0 < ASensorEventQueue_getEvents(data.sensor_event_queue,
                                                      &event, 1))) {
                {
                  auto mag(std::sqrt(
                      ((event.acceleration.x * event.acceleration.x) +
                       (event.acceleration.y * event.acceleration.y) +
                       (event.acceleration.z * event.acceleration.z))));
                  m_mag[m_mag_idx] = mag;
                  m_mag_idx = ((m_mag_idx + 1) % M_MAG_N);
                  if ((0 == (m_mag_idx % 1))) {
                    for (unsigned int i = 0; (i < M_MAG_N); i += 1) {
                      m_mag2[i] = m_mag[i];
                    }
                    app->redrawNeeded = 1;
                    __android_log_print(ANDROID_LOG_INFO, "native-activity",
                                        "a: %f %f %f", m_mag[0], m_mag[1],
                                        m_mag[2]);
                  }
                }
              }
            }
            break;
          }
          default: {
            if ((nullptr != source)) {
              source->process(app, source);
            }
            break;
          }
          }
        }
        if (app->redrawNeeded) {
          __android_log_print(ANDROID_LOG_INFO, "native-activity", "draw %d",
                              app->redrawNeeded);
          drawSomething(app);
          app->redrawNeeded = 0;
          __android_log_print(ANDROID_LOG_INFO, "native-activity", "dree %d",
                              app->redrawNeeded);
        }
      }
    }
  }
}