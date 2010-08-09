/*!
  \file
  \brief Plotter (SDL)

  \author Satofumi KAMIMURA

  \todo glDrawElements() を使うように修正する

  $Id$
*/

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include "plotter_sdl.h"
#include <SDL.h>
#include <SDL_opengl.h>


enum {
    SCREEN_WIDTH = 640,
    SCREEN_HEIGHT = 480,

    MAX_POINTS = 1081,
};


typedef struct
{
    GLfloat x;
    GLfloat y;
} vector_t;


static SDL_Surface *screen = NULL;
static vector_t points[MAX_POINTS];
static size_t points_size = 0;
static GLuint buffer_id = 0;
static double draw_magnify = 0.1;


static void opengl_initialize(void)
{
    int bpp = SDL_GetVideoInfo()->vfmt->BitsPerPixel;

    // Initialize the display
    int rgb_size[3];
    switch (bpp) {
    case 8:
        rgb_size[0] = 3;
        rgb_size[1] = 3;
        rgb_size[2] = 2;
        break;

    case 15:
    case 16:
        rgb_size[0] = 5;
        rgb_size[1] = 5;
        rgb_size[2] = 5;
        break;

    default:
        rgb_size[0] = 8;
        rgb_size[1] = 8;
        rgb_size[2] = 8;
        break;
    }
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, rgb_size[0]);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, rgb_size[1]);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, rgb_size[2]);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
}


static void opengl_setup(void)
{
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(-1, -1, +1, +1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, +1.0, -1.0, +1.0, -10.0, +10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
}


static void draw_points(void)
{
    int memory_size;
    if (points_size <= 0) {
        return;
    }

    memory_size = points_size * sizeof(points[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    glBufferData(GL_ARRAY_BUFFER, memory_size, points, GL_STATIC_DRAW);

    glInterleavedArrays(GL_V2F, 0, NULL);
    glDrawArrays(GL_POINTS, 0, points_size);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    points_size = 0;
}


static void enter2D(void)
{
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0, SCREEN_WIDTH - 1.0, 0.0, SCREEN_HEIGHT - 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glTranslatef(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0);
}


bool plotter_initialize(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    // 画面の作成
    opengl_initialize();
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_OPENGL);
    if (!screen) {
        return false;
    }
    opengl_setup();

    // 描画設定
    glPointSize(2.0);
    glGenBuffers(1, &buffer_id);
    enter2D();

    return true;
}


void plotter_terminate(void)
{
    SDL_Quit();
}


void plotter_clear(void)
{
    glClearColor(0x00, 0x00, 0x00, 0xff);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void plotter_swap(void)
{
    // 表示を入れ換えるときに、まだ描画していない内容を描画する
    draw_points();

    SDL_GL_SwapBuffers();
}


void plotter_set_color(unsigned char r, unsigned g, unsigned b)
{
    // 色を変更するときに、まとめて描画を行う
    draw_points();

    glColor3f(r / 255.0, g / 255.0, b / 255.0);
}


void plotter_plot(float x, float y)
{
    if (points_size >= MAX_POINTS) {
        return;
    }

    points[points_size].x = draw_magnify * x;
    points[points_size].y = draw_magnify * y;
    ++points_size;
}


bool plotter_is_quit(void)
{
    bool is_quit = false;
    int magnify = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_QUIT:
            is_quit = true;
            break;

        case SDL_KEYDOWN:
            if ((event.key.keysym.sym == SDLK_q) ||
                (event.key.keysym.sym == SDLK_F4)) {
                is_quit = true;
            }
            if (event.key.keysym.sym == SDLK_COMMA) {
                --magnify;
            }
            if (event.key.keysym.sym == SDLK_PERIOD) {
                ++magnify;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_WHEELUP) {
                --magnify;
            } else if (event.button.button == SDL_BUTTON_WHEELDOWN) {
                ++magnify;
            }
            break;
        }
    }

    // 描画の拡大率を変更する
    while (magnify < 0) {
        draw_magnify *= 0.90;
        ++magnify;
    }
    while (magnify > 0) {
        draw_magnify *= 1.10;
        --magnify;
    }
    if (draw_magnify < 0.001) {
        draw_magnify = 0.001;
    } else if (draw_magnify > 10.0) {
        draw_magnify = 10.0;
    }

    return is_quit;
}
