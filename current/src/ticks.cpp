/*!
  \file
  \brief タイムスタンプの取得

  \author Satofumi KAMIMURA

  $Id$
*/

#include "ticks.h"
#include "detect_os.h"
#include <time.h>


long qrk::ticks(void)
{
    static bool is_initialized = false;
#if defined(QRK_WINDOWS_OS)
    clock_t current_clock;
#else
    static struct timespec first_spec;
    struct timespec current_spec;
#endif
    long msec_time;

#if defined(QRK_WINDOWS_OS)
    if (!is_initialized) {
        is_initialized = true;
    }
    current_clock = clock();
    msec_time = current_clock / (CLOCKS_PER_SEC / 1000);
#else
    if (!is_initialized) {
        clock_gettime(CLOCK_REALTIME, &first_spec);
        is_initialized = true;
    }
    clock_gettime(CLOCK_REALTIME, &current_spec);
    msec_time =
        (current_spec.tv_sec - first_spec.tv_sec) * 1000
        + (current_spec.tv_nsec - first_spec.tv_nsec) / 1000000;
#endif
    return msec_time;
}
