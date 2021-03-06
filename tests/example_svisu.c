#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "visu/svisu.h"

float data[10][4] = {
        {0,  0,  0, 1},
        {10, 0,  0, 1},

        {20, 0,  0, 1},
        {30, 0,  0, 1},
        {0,  20, 0, 1},
        {10, 20, 0, 1},
        {20, 20, 0, 1},
        {30, 20, 0, 1},
        {0,  10, 0, 1},
        {30, 10, 0, 1},
};

float strip[91][4] = {{-23.2398167,  -79.6182175,  208, 1},
                      {-25.6511745,  -79.7094269,  208, 1},
                      {-28.0644016,  -79.6210785,  208, 1},
                      {-31.0063057,  -79.4405899,  208, 1},
                      {-33.9179115,  -79.0024567,  208, 1},
                      {-36.8054428,  -78.3719406,  208, 1},
                      {-39.6552658,  -77.5798111,  208, 1},
                      {-42.4537582,  -76.5980453,  208, 1},
                      {-45.1765785,  -75.3859406,  208, 1},
                      {-47.8222847,  -74.0314102,  208, 1},
                      {-50.3350372,  -72.4589081,  208, 1},
                      {-52.7338257,  -70.7151184,  208, 1},
                      {-55.0044785,  -68.8032379,  208, 1},
                      {-57.0947037,  -66.7000885,  208, 1},
                      {-59.0828743,  -64.4797668,  208, 1},
                      {-60.9500732,  -62.163929,   208, 1},
                      {-62.6219254,  -59.7113342,  208, 1},
                      {-64.0820847,  -57.1249161,  208, 1},
                      {-65.4158173,  -54.4792061,  208, 1},
                      {-66.491539,   -51.7133369,  208, 1},
                      {-67.4036942,  -48.8932724,  208, 1},
                      {-68.0571518,  -46.0292816,  208, 1},
                      {-68.5637207,  -43.1404343,  208, 1},
                      {-68.8833237,  -40.2096443,  208, 1},
                      {-68.975914,   -37.2691154,  208, 1},
                      {-68.8207169,  -34.3098068,  208, 1},
                      {-68.5449982,  -31.3611565,  208, 1},
                      {-68.0440063,  -28.4304504,  208, 1},
                      {-67.2802582,  -25.5529671,  208, 1},
                      {-66.321846,   -22.748806,   208, 1},
                      {-65.2622986,  -19.9778481,  208, 1},
                      {-63.9383545,  -17.3611126,  208, 1},
                      {-62.4380379,  -14.8651705,  208, 1},
                      {-60.7816963,  -12.4659529,  208, 1},
                      {-59.0089417,  -10.1749926,  208, 1},
                      {-57.0099258,  -8.06512737,  208, 1},
                      {-54.9405022,  -6.10545111,  208, 1},
                      {-52.8208542,  -4.18452978,  208, 1},
                      {-50.5386124,  -2.51932096,  208, 1},
                      {-48.1066589,  -1.06372321,  208, 1},
                      {-45.5515823,  0.219340652,  208, 1},
                      {-42.9290771,  1.39790416,   208, 1},
                      {-40.1581421,  2.36462355,   208, 1},
                      {-37.3549385,  3.18340826,   208, 1},
                      {-34.5308037,  3.91554022,   208, 1},
                      {-31.6764927,  4.51159811,   208, 1},
                      {-28.8080349,  4.84940386,   208, 1},
                      {-25.932476,   4.81039953,   208, 1},
                      {-23.0606899,  4.62327909,   208, 1},
                      {-20.2189598,  4.1559763,    208, 1},
                      {-17.3954563,  3.45381308,   208, 1},
                      {-14.5555782,  2.66889334,   208, 1},
                      {-11.7519865,  1.79005432,   208, 1},
                      {-9.02248669,  0.711368382,  208, 1},
                      {-6.39104223,  -0.584810555, 208, 1},
                      {-3.86754107,  -2.09356451,  208, 1},
                      {-1.43735421,  -3.79544258,  208, 1},
                      {0.82252723,   -5.66862202,  208, 1},
                      {2.91647816,   -7.72227573,  208, 1},
                      {4.92167282,   -9.85604668,  208, 1},
                      {6.81020069,   -12.0908566,  208, 1},
                      {8.41643143,   -14.491641,   208, 1},
                      {9.91432571,   -16.9963722,  208, 1},
                      {11.2645092,   -19.5784454,  208, 1},
                      {12.4225864,   -22.2651253,  208, 1},
                      {13.3418026,   -25.0567379,  208, 1},
                      {14.1405773,   -27.9101562,  208, 1},
                      {14.755084,    -30.808094,   208, 1},
                      {15.1365204,   -33.7382927,  208, 1},
                      {15.2830172,   -36.6921616,  208, 1},
                      {15.248642,    -39.6451607,  208, 1},
                      {14.9633913,   -42.5822563,  208, 1},
                      {14.4486494,   -45.502739,   208, 1},
                      {13.7395678,   -48.3837395,  208, 1},
                      {12.9463596,   -51.2314682,  208, 1},
                      {11.8860979,   -53.968708,   208, 1},
                      {10.6894302,   -56.6053848,  208, 1},
                      {9.33175468,   -59.1612663,  208, 1},
                      {7.81375599,   -61.6178589,  208, 1},
                      {5.9851346,    -63.9034615,  208, 1},
                      {4.04006624,   -66.1004105,  208, 1},
                      {1.9491837,    -68.2137604,  208, 1},
                      {-0.301050186, -70.174675,   208, 1},
                      {-2.71621943,  -71.9169693,  208, 1},
                      {-5.20477486,  -73.5512009,  208, 1},
                      {-7.79418659,  -75.0106201,  208, 1},
                      {-10.4951677,  -76.2525101,  208, 1},
                      {-13.2790098,  -77.2862091,  208, 1},
                      {-17.1130257,  -78.434639,   208, 1},
                      {-19.5680332,  -79.015358,   208, 1},
                      {-22.0423412,  -79.410553,   208, 1}};


void trackbar_callback(SVisu *sv, double value, void *callback_user_data) {
    printf("tracky! value: %f\n", value);
}

void mouse_ray_callback(SVisu *sv, vec3 ray_pos, vec3 ray_dir, void *user_data) {
    printf("ray!\n - pos{%f, %f, %f}\n - dir{%f, %f, %f}\n",
           ray_pos.x, ray_pos.y, ray_pos.z,
           ray_dir.x, ray_dir.y, ray_dir.z);
}

void render_callback(SVisu *sv, void *user_data) {
    static int cnt = 0;
    cnt++;
    if (cnt % 60 == 0)
        puts("render!");
}

int main() {
    puts("Start");

    pCloud points = {(vec4*) data, 10};
    pCloud linestrip = {(vec4*) strip, 91};

    SVisu *sv = svisu_new(false, 10);
    svisu_set_background_color(sv, VU_WHITE.rgb, VU_GREEN.rgb, VU_BLUE.rgb, VU_RED.rgb);

    sv->mouse_ray_callback = mouse_ray_callback;
    sv->render_callback = render_callback;

    svisu_add_trackbar(sv, (SVisuTrackbar) {"tracky", 0, 100, 2, 50, trackbar_callback});

    svisu_points(sv, points, (vec4) {{1, 0, 1, 1}}, 10);

    svisu_sphere(sv, (vec3) {{300, 0, 0}}, 50, VU_GREEN);
    svisu_arrow_swapped(sv, (vec3) {{-300, 0, 0}}, (vec3) {{100, 0, 0}}, 2, VU_CYAN);

    svisu_lines(sv, linestrip, VU_RED, 1, VU_LINE_MODE_LINE_LOOP);

    char key = svisu_show(sv);
    printf("key: %c\n", key);

    svisu_kill(&sv);

    puts("fin");
}
