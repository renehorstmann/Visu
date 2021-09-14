#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <pbase/mathc/mathc.h>
#include "visu/svisu.h"


static void source_x(SVisu *sv, double value, void *callback_user_data) {
    VuRawMesh *ro = callback_user_data;
    ro->light_options.light_source.x = value;
}
static void source_y(SVisu *sv, double value, void *callback_user_data) {
    VuRawMesh *ro = callback_user_data;
    ro->light_options.light_source.y = value;
}
static void source_z(SVisu *sv, double value, void *callback_user_data) {
    VuRawMesh *ro = callback_user_data;
    ro->light_options.light_source.z = value;
}
static void color(SVisu *sv, double value, void *callback_user_data) {
    VuRawMesh *ro = callback_user_data;
    ro->light_options.light_color = vec3_set(value);
}

static void power(SVisu *sv, double value, void *callback_user_data) {
    VuRawMesh *ro = callback_user_data;
    ro->light_options.light_power = value;
}

void on_sphere_kill(struct VuRenderObject *a, void *b) {
    puts("sphere was killed");
}

int main() {
    puts("Start");

    SVisu *sv = svisu_new(true, 100);

    VuRawMesh *ro = svisu_sphere(sv, (vec3) {{0, 0, 0}}, 100, SV_WHITE);
    ro->super.on_kill_callback = on_sphere_kill;

    svisu_add_trackbar(sv, (SVisuTrackbar) {"x", -1, 1, 0.01, VU_LIGHT_SOURCE_INIT.x, source_x, ro});
    svisu_add_trackbar(sv, (SVisuTrackbar) {"y", -1, 1, 0.01, VU_LIGHT_SOURCE_INIT.y, source_y, ro});
    svisu_add_trackbar(sv, (SVisuTrackbar) {"z", -1, 1, 0.01, VU_LIGHT_SOURCE_INIT.z, source_z, ro});

    svisu_add_trackbar(sv, (SVisuTrackbar) {"color", 0, 1, 0.01, 1, color, ro});
    svisu_add_trackbar(sv, (SVisuTrackbar) {"power", 0, 2, 0.01, VU_LIGHT_POWER_INIT, power, ro});

    svisu_show(sv);

    svisu_kill(&sv);

    puts("fin");
}
