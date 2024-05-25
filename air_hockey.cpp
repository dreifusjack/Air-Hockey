/* air_hockey.cpp ---
 *
 * Filename: air_hockey.cpp
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
#include "key.hpp"
#include "ball.hpp"
#include "air_hockey.hpp"
#include <ncurses.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

// Main Game function
void air_hockey()
{
  int zone_height, zone_width;
  struct timespec tim = {0, 200000000};
  struct timespec tim_ret;
  int arrow, c;
  int slider_x_speed = 5, slider_y_speed = 5;
  initscr();
  getmaxyx(stdscr, zone_height, zone_width);
  zone_height -= 1;
  zone_width -= 1;
  zone_t *z = init_zone(0, 1, zone_width, zone_height - 1);
  ball_t *b = init_ball(zone_width / 2, zone_height / 2, 1, 1);
  slider_t *top = init_slider(zone_width / 2, 5, 'T');
  slider_t *bottom = init_slider(zone_width / 2, zone_height - 5, 'U');
  draw_zone(z);
  draw_slider(top);
  draw_slider(bottom);
  draw_ball(b);
  refresh();
  // nodelay(stdscr, TRUE); // Do not wait for characters using getch.
  halfdelay(5); // Wait for 1/10th of a second for user input
  noecho();
  bool is_paused = false;
  int pause_input;

  string pause_instruction = "Press 'P' to pause/resume the game.";
  mvprintw(0, (zone_width - pause_instruction.length()) / 2, pause_instruction.c_str());
  refresh();

  while (1)
  {
    // Check for user input
    nodelay(stdscr, TRUE); // Non-blocking getch
    pause_input = getch();

    if (pause_input == 'P' || pause_input == 'p')
    {
      is_paused = !is_paused;
    }

    if (is_paused)
    {
      // Display "Game Paused" message
      string pause_message = "Game Paused. Press 'P' to resume.";
      mvprintw(zone_height / 2, (zone_width - pause_message.length()) / 2, pause_message.c_str());
      refresh();

      // Wait for 'P' or 'p' to resume
      while (true)
      {
        pause_input = getch();
        if (pause_input == 'P' || pause_input == 'p')
        {
          is_paused = false;
          break;
        }
      }
      // Clear the "Game Paused" message
      clear();
      draw_zone(z);
      draw_slider(top);
      draw_slider(bottom);
      draw_ball(b);
      refresh();
    }
    // Move the current piece
    if ((arrow = read_escape(&c)) != NOCHAR)
    {
      switch (arrow)
      {

      // Functionality for Left Arrow Key that controls the bottom slider
      case LEFT:
        moveSlider(bottom, bottom->upper_left_x - slider_x_speed, bottom->upper_left_y, zone_width, zone_height);
        refresh();
        break;
      // Functionality for Rigth Arrow Key that controls the bottom slider
      case RIGHT:
        moveSlider(bottom, bottom->upper_left_x + slider_x_speed, bottom->upper_left_y, zone_width, zone_height);
        break;
      // Functionality for Up Arrow Key that controls the bottom slider
      case UP:
        moveSlider(bottom, bottom->upper_left_x, bottom->upper_left_y - slider_y_speed, zone_width, zone_height);
        break;
      // Functionality for Down Arrow Key that controls the bottom slider
      case DOWN:
        moveSlider(bottom, bottom->upper_left_x, bottom->upper_left_y + slider_y_speed, zone_width, zone_height);
        break;
      // Functionality for A Key that controls the top slider
      case A:
        moveSlider(top, top->upper_left_x - slider_x_speed, top->upper_left_y, zone_width, zone_height);
        break;
        // Functionality for D Key that controls the top slider
      case D:
        moveSlider(top, top->upper_left_x + slider_x_speed, top->upper_left_y, zone_width, zone_height);
        break;
      // Functionality for W Key that controls the top slider
      case W:
        moveSlider(top, top->upper_left_x, top->upper_left_y - slider_y_speed, zone_width, zone_height);
        break;
      // Functionality for S Key that controls the top slider
      case S:
        moveSlider(top, top->upper_left_x, top->upper_left_y + slider_y_speed, zone_width, zone_height);
        break;
      default:
        mvprintw(1, 5, "%c", c);
        break;
      }
    }
    refresh();
    undraw_zone(z);
    draw_zone(z);
    undraw_ball(b);
    moveBall(b);
    checkCollisionSlider(bottom, b);
    checkCollisionSlider(top, b);
    checkCollisionWithZone(b, z);
    draw_ball(b);
    refresh();
    // usleep(200000);
    nanosleep(&tim, &tim_ret);
  }
}
