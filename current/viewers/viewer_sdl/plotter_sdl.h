#ifndef PLOTTER_SDL_H
#define PLOTTER_SDL_H

/*!
  \file
  \brief Plotter (SDL)

  \author Satofumi KAMIMURA

  $Id$
*/

#include <stdbool.h>


extern bool plotter_initialize(void);
extern void plotter_terminate(void);
extern void plotter_clear(void);
extern void plotter_swap(void);
extern void plotter_set_color(unsigned char r, unsigned g, unsigned b);
extern void plotter_plot(long x, long y);
extern bool plotter_is_quit(void);

#endif /* !PLOTTER_SDL_H */
