#include "android_native_app_glue.h"
#include <algorithm>
#include <android/log.h>
#include <android/sensor.h>
#include <array>
#include <stdio.h>
#include <sys/time.h>
enum Constants { M_MAG_N = 1024 };

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
    (0.0e+0f), (0.0e+0f)};
int m_mag_idx = 0;
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
    (0.0e+0f), (0.0e+0f)};

struct userdata_t {
  int32_t move_x;
  const ASensor *sensor_accelerometer;
  const ASensor *sensor_magnetic_field;
  const ASensor *sensor_gyroscope;
  ASensorEventQueue *sensor_event_queue;
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
        // FIXME why conversion from float to pos_x here?;
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
               float y2, int r, int g, int b, int a) {
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
          set_pixel(buf, y, x, r, g, b, a);
        } else {
          set_pixel(buf, x, y, r, g, b, a);
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
      memset(buf.bits, 0xFFFFFFFF,
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
              auto yy1((sizeof(uint32_t) * (((y + 1)) % buf.height)));
              auto yy0((sizeof(uint32_t) * (((y + 0)) % buf.height)));
              draw_line(buf, xx0, (yy0 / 4), xx1, (yy1 / 4), 255, 0, 0, 255);
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
        {
          auto min_delay(ASensor_getMinDelay(data->sensor_accelerometer));
          __android_log_print(ANDROID_LOG_INFO, "native-activity",
                              "accelerometer_min_delay = %d", min_delay);
          // setting rate only after sensor is enabled;
          ASensorEventQueue_setEventRate(data->sensor_event_queue,
                                         data->sensor_accelerometer, min_delay);
        }
      }
      if ((nullptr != data->sensor_magnetic_field)) {
        ASensorEventQueue_enableSensor(data->sensor_event_queue,
                                       data->sensor_magnetic_field);
        {
          auto min_delay(ASensor_getMinDelay(data->sensor_magnetic_field));
          __android_log_print(ANDROID_LOG_INFO, "native-activity",
                              "magnetic_field_min_delay = %d", min_delay);
          // setting rate only after sensor is enabled;
          ASensorEventQueue_setEventRate(
              data->sensor_event_queue, data->sensor_magnetic_field, min_delay);
        }
      }
      if ((nullptr != data->sensor_gyroscope)) {
        ASensorEventQueue_enableSensor(data->sensor_event_queue,
                                       data->sensor_gyroscope);
        {
          auto min_delay(ASensor_getMinDelay(data->sensor_gyroscope));
          __android_log_print(ANDROID_LOG_INFO, "native-activity",
                              "gyroscope_min_delay = %d", min_delay);
          // setting rate only after sensor is enabled;
          ASensorEventQueue_setEventRate(data->sensor_event_queue,
                                         data->sensor_gyroscope, min_delay);
        }
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
      if ((nullptr != data->sensor_magnetic_field)) {
        ASensorEventQueue_disableSensor(data->sensor_event_queue,
                                        data->sensor_magnetic_field);
      }
      if ((nullptr != data->sensor_gyroscope)) {
        ASensorEventQueue_disableSensor(data->sensor_event_queue,
                                        data->sensor_gyroscope);
      }
    }
    break;
  }
  }
}
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
class net_t {
private:
  int m_fd;
  int m_conn_fd;
  sockaddr_in m_addr;

public:
  net_t(int port = 1234) {
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((!((0 < bind(m_fd, reinterpret_cast<sockaddr *>(&m_addr),
                     sizeof(m_addr)))))) {
      __android_log_print(ANDROID_LOG_INFO, "native-activity", "assertion (< 0\
             (funcall bind m_fd (funcall reinterpret_cast<sockaddr*> &m_addr)\
                      (funcall sizeof m_addr))) failed");
    }
    if ((!((0 < listen(m_fd, 10))))) {
      __android_log_print(ANDROID_LOG_INFO, "native-activity",
                          "assertion (< 0 (funcall listen m_fd 10)) failed");
    }
  }
  ~net_t() { close(); }
  void accept() {
    {
      socklen_t len(sizeof(m_addr));
      m_conn_fd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&m_addr), &len);
      if ((!((0 < m_conn_fd)))) {
        __android_log_print(ANDROID_LOG_INFO, "native-activity",
                            "assertion (< 0 m_conn_fd) failed");
      }
    }
  }
  void close() { ::close(m_fd); }
  template <std::size_t N> int send(std::array<char, N> data) {
    {
      auto bytes_sent(0);
      auto bytes_left(N);
      while ((0 < bytes_left)) {
        {
          auto bytes(::send(m_conn_fd, (&(data[bytes_sent])), bytes_left, 0));
          if ((bytes < 0)) {
            {
              const char *err_msgs[15] = {
                  "The socket's  file  descriptor  is  marked  O_NONBLOCK  and "
                  " the requested operation would block.",
                  "A connection was forcibly closed by peer.",
                  "A connection was forcibly closed by a peer.",
                  "The socket is not connection-mode and no peer address is "
                  "set.",
                  "A signal interrupted send() before any data was "
                  "transmitted.",
                  "The  message  is too large to be sent all at once, as the "
                  "socket requires.",
                  "The socket is not connected.",
                  "The socket argument does not refer to a socket.",
                  "The socket argument is associated with a socket  that  does "
                  " not support one or more of the values set in flags.",
                  "The  socket  is  shut down for writing, or the socket is "
                  "connection-mode and is no longer connected. In the latter "
                  "case, and if the  socket  is  of  type  SOCK_STREAM or "
                  "SOCK_SEQPACKET and the MSG_NOSIGNAL flag is not set, the "
                  "SIGPIPE signal is generated to the calling thread.",
                  "The calling process does not have appropriate privileges.",
                  "An  I/O error occurred while reading from or writing to the "
                  "file system.",
                  "The local network interface used to  reach  the  "
                  "destination  is down.",
                  "No route to the network is present.",
                  "Insufficient  resources  were available in the system to "
                  "perform the operation."};
              auto err_msg_lut([&](ssize_t idx) -> int {
                switch (idx) {
                case EAGAIN: {
                  return 0;
                  break;
                }
                case EBADF: {
                  return 1;
                  break;
                }
                case ECONNRESET: {
                  return 2;
                  break;
                }
                case EDESTADDRREQ: {
                  return 3;
                  break;
                }
                case EINTR: {
                  return 4;
                  break;
                }
                case EMSGSIZE: {
                  return 5;
                  break;
                }
                case ENOTCONN: {
                  return 6;
                  break;
                }
                case ENOTSOCK: {
                  return 7;
                  break;
                }
                case EOPNOTSUPP: {
                  return 8;
                  break;
                }
                case EPIPE: {
                  return 9;
                  break;
                }
                case EACCES: {
                  return 10;
                  break;
                }
                case EIO: {
                  return 11;
                  break;
                }
                case ENETDOWN: {
                  return 12;
                  break;
                }
                case ENETUNREACH: {
                  return 13;
                  break;
                }
                case ENOBUFS: {
                  return 14;
                  break;
                }
                }
              });
              __android_log_print(ANDROID_LOG_INFO, "native-activity",
                                  "net::send errno=%d", errno,
                                  err_msgs[err_msg_lut(errno)]);
            }
          }
          bytes_left -= bytes;
          bytes_sent += bytes;
        }
      }
      return bytes_sent;
    }
  }
};

#include <condition_variable>
#include <deque>
#include <thread>
class mt_buffer_t {
private:
  std::mutex m_mutex;
  std::condition_variable m_cond;
  std::deque<std::array<char, 59>> m_buffer;
  const unsigned int m_size = 10;

public:
  void add(const std::array<char, 59> &num) {
    while (true) {
      {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_cond.wait(locker, [this]() { return (m_buffer.size() < m_size); });
        m_buffer.push_back(num);
        locker.unlock();
        m_cond.notify_all();
        return;
      }
    }
  }
  std::array<char, 59> remove() {
    while (true) {
      {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_cond.wait(locker, [this]() { return (0 < m_buffer.size()); });
        {
          auto back(m_buffer.back());
          m_buffer.pop_back();
          locker.unlock();
          m_cond.notify_all();
          return back;
        }
      }
    }
  }
};

void mt_consumer(mt_buffer_t &buffer) {
  {
    net_t net;
    __android_log_print(ANDROID_LOG_INFO, "native-activity",
                        "mt_consumer thread started");
    net.accept();
    __android_log_print(ANDROID_LOG_INFO, "native-activity",
                        "mt_consumer accepted connection");
    while (true) {
      {
        auto value(buffer.remove());
        net.send(value);
      }
    }
    __android_log_print(ANDROID_LOG_INFO, "native-activity",
                        "mt_consumer finished");
  }
}
void mt_produce(mt_buffer_t &buffer, const std::array<char, 59> &value) {
  buffer.add(value);
}
void android_main(android_app *app) {
  app_dummy();
  {
    mt_buffer_t mt_buffer;
    std::thread consumer1(mt_consumer, std::ref(mt_buffer));
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
      if ((nullptr != data.sensor_accelerometer)) {
        __android_log_print(
            ANDROID_LOG_INFO, "native-activity",
            "error in getDefaultSensor data.sensor_accelerometer");
      }
      data.sensor_magnetic_field = ASensorManager_getDefaultSensor(
          sensor_manager, ASENSOR_TYPE_MAGNETIC_FIELD);
      if ((nullptr != data.sensor_magnetic_field)) {
        __android_log_print(
            ANDROID_LOG_INFO, "native-activity",
            "error in getDefaultSensor data.sensor_magnetic_field");
      }
      data.sensor_gyroscope = ASensorManager_getDefaultSensor(
          sensor_manager, ASENSOR_TYPE_GYROSCOPE);
      if ((nullptr != data.sensor_gyroscope)) {
        __android_log_print(ANDROID_LOG_INFO, "native-activity",
                            "error in getDefaultSensor data.sensor_gyroscope");
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
                                    &events,
                                    reinterpret_cast<void **>(&source)));
          if ((0 <= ident)) {
            switch (ident) {
            case LOOPER_ID_USER: {
              {
                ASensorEvent event;
                while ((0 < ASensorEventQueue_getEvents(data.sensor_event_queue,
                                                        &event, 1))) {
                  switch (event.type) {
                  case ASENSOR_TYPE_ACCELEROMETER: {
                    // timestamp is in nanoseconds;
                    {
                      static std::array<char, 59> s;
                      snprintf(s.data(), 59,
                               "acc %12lld %+12.5f %+12.5f %+12.5f\n",
                               static_cast<long long>(event.timestamp),
                               event.acceleration.x, event.acceleration.y,
                               event.acceleration.z);
                      mt_produce(std::ref(mt_buffer), s);
                    }
                    break;
                  }
                  case ASENSOR_TYPE_GYROSCOPE: {
                    // timestamp is in nanoseconds;
                    {
                      static std::array<char, 59> s;
                      snprintf(s.data(), 59,
                               "gyr %12lld %+12.5f %+12.5f %+12.5f\n",
                               static_cast<long long>(event.timestamp),
                               event.data[0], event.data[1], event.data[2]);
                      mt_produce(std::ref(mt_buffer), s);
                    }
                    break;
                  }
                  case ASENSOR_TYPE_MAGNETIC_FIELD: {
                    // timestamp is in nanoseconds;
                    {
                      static std::array<char, 59> s;
                      snprintf(
                          s.data(), 59, "mag %12lld %+12.5f %+12.5f %+12.5f\n",
                          static_cast<long long>(event.timestamp),
                          event.magnetic.x, event.magnetic.y, event.magnetic.z);
                      mt_produce(std::ref(mt_buffer), s);
                    }
                    break;
                  }
                  }
                  if ((event.type == ASENSOR_TYPE_GYROSCOPE)) {
                    {
                      auto mag(event.data[2]);
                      m_mag[m_mag_idx] = mag;
                      m_mag_idx = ((m_mag_idx + 1) % M_MAG_N);
                      if ((0 == (m_mag_idx % 8))) {
                        for (unsigned int i = 0; (i < M_MAG_N); i += 1) {
                          m_mag2[i] = m_mag[i];
                        }
                        app->redrawNeeded = 1;
                      }
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
            drawSomething(app);
            app->redrawNeeded = 0;
          }
        }
      }
    }
  }
}