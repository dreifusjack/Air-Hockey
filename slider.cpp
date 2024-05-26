/* slider.cpp ---
 *
 * Filename: slider.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Wed May 15 2024
 * Last-Updated:
 *           By:
 *     Update #: 0
 * Keywords:
 * Compatibility:
 *
 */

/* Copyright (c) 2016 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */
#include "slider.hpp"
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>

// This intializes the properties of the slider. Is by default size 4.
const slider_t slider_types[2] = {
    {"top", {{1, 1, 1, 1, 0, 0, 0}}, 0, 0, '%', 2, 4},
    {"bottom", {{1, 1, 1, 1, 0, 0, 0}}, 0, 0, '+', 3, 4},
};

// Changes the location of the slider
void moveSlider(slider_t *s, int x, int y, int zone_width, int zone_height)
{
  // if trying to move top slider below the center line
  if (s->type_str[0] == 't' && y > (zone_height / 2))
  {
    return;
  }
  // if trying to move bottom slider above the center line
  if (s->type_str[0] == 'b' && y < (zone_height / 2))
  {
    return;
  }
  // if the slider goes outside of the padding for the zone (left and top)
  if (x < 9 || y < 3)
  {
    return;
  }
  // if the slider goes outside the zone with (bottom and right)
  if (x + 4 > zone_width || y + 1 > zone_height)
  {
    return;
  }
  undraw_slider(s);
  s->upper_left_x = x;
  s->upper_left_y = y;
  draw_slider(s);
}

// Intializes the slider
slider_t *init_slider(int initial_x, int initial_y, char type, int size)
{
  slider_t *s = (slider *)malloc(sizeof(slider_t));
  if (type == 'T')
  {
    memcpy(s, &slider_types[0], sizeof(slider_t));
  }
  else
  {
    memcpy(s, &slider_types[1], sizeof(slider_t));
  }

  s->upper_left_x = initial_x;
  s->upper_left_y = initial_y;
  s->size = size;
  // adjust the size of the piece if it is greater than 4 (the default)
  for (int i = 0; i < size; i++)
    s->piece[0][i] = 1;
  return (s);
}

// Renders the slider on the screen
void draw_slider(slider_t *s)
{
  int x, y;

  attron(COLOR_PAIR(s->color_pair)); // add colors

  for (x = 0; x < s->size; x++)
  {
    for (y = 0; y < 1; y++)
    {
      if (s->piece[y][x])
      {
        mvprintw(s->upper_left_y + y, s->upper_left_x + x, "%c", s->draw_char);
      }
    }
  }

  attroff(COLOR_PAIR(s->color_pair)); // reset colors
}

// Replaces the slider with blank spaces
void undraw_slider(slider_t *s)
{
  int x, y;
  for (x = 0; x < s->size; x++)
  {
    for (y = 0; y < 1; y++)
    {
      if (s->piece[y][x])
      {
        mvprintw(s->upper_left_y + y, s->upper_left_x + x, " ", s->draw_char);
      }
    }
  }
}
