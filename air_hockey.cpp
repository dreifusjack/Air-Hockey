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
#include <chrono>
using namespace std;

// Main Game function
void air_hockey()
{
  int zone_height, zone_width;
  struct timespec tim = {0, 200000000};
  struct timespec tim_ret;
  int arrow, c;
  int slider_x_speed = 4, slider_y_speed = 4;
  initscr();
  getmaxyx(stdscr, zone_height, zone_width);
  zone_height -= 1;
  zone_width -= 1;

  // get the slider size from the user before starting the game
  int slider_size;
  do
  {
    mvprintw(zone_height / 2, (zone_width - 30) / 2, "Enter a slider size (4-7): ");
    slider_size = getch();
  } while (slider_size < 52 || slider_size > 55); // using ascii values for the numbers 4-7
  // convert from ascii value to the correlated value, where 52 = 4, so 52-48 = 4
  slider_size -= 48;

  bool game_over = false;
  while (!game_over)
  {
    clear();
    zone_t *z = init_zone(0, 2, zone_width, zone_height - 2);
    ball_t *b = init_ball(zone_width / 2, zone_height / 2, 1, 1);
    slider_t *top = init_slider(zone_width / 2, 5, 'T', slider_size);
    slider_t *bottom = init_slider(zone_width / 2, zone_height - 5, 'U', slider_size);
    draw_zone(z);
    draw_slider(top);
    draw_slider(bottom);
    draw_ball(b);
    refresh();
    // nodelay(stdscr, TRUE); // Do not wait for characters using getch.
    halfdelay(5); // Wait for 1/2th of a second for user input
    noecho();
    bool is_paused = false;
    int in_game_input;

    string pause_instruction = "Press 'P' to pause/resume the game.";
    mvprintw(0, (zone_width - pause_instruction.length()) / 2, pause_instruction.c_str());
    refresh();

    string end_instruction = "Press 'Q' to end the game.";
    mvprintw(1, (zone_width - end_instruction.length()) / 2, end_instruction.c_str());
    refresh();

    // timer for the game
    auto start_time = chrono::steady_clock::now();
    auto end_time = start_time + chrono::minutes(2);

    bool in_game = true;

    while (in_game)
    {
      // Check for user input
      nodelay(stdscr, TRUE); // Non-blocking getch
      in_game_input = getch();

      if (in_game_input == 'P' || in_game_input == 'p')
      {
        is_paused = !is_paused;
      }
      else if (in_game_input == 'Q' || in_game_input == 'q')
      {
        in_game = false;
        end_game_screen(zone_width, zone_height);
        int end_choice;
        while (true)
        {
          end_choice = getch();
          if (end_choice == 'N' || end_choice == 'n')
          {
            clear();
            refresh();
            break; // Breaks out of the end game screen loop and restarts the game
          }
          else if (end_choice == 'E' || end_choice == 'e')
          {
            clear();
            printw("Exiting the game...");
            game_over = true;
            in_game = false;
            break; // Breaks out of both loops and exits the program
          }
        }
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
          in_game_input = getch();
          if (in_game_input == 'P' || in_game_input == 'p')
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

      auto current_time = chrono::steady_clock::now();
      auto time_left = chrono::duration_cast<chrono::seconds>(end_time - current_time).count();
      if (time_left <= 0)
      {
        in_game = false;
        end_game_screen(zone_width, zone_height);
        int end_choice;
        while (true)
        {
          end_choice = getch();
          if (end_choice == 'N' || end_choice == 'n')
          {
            clear();
            refresh();
            break; // Breaks out of the end game screen loop and restarts the game
          }
          else if (end_choice == 'E' || end_choice == 'e')
          {
            game_over = true;
            in_game = false;
            break; // Breaks out of both loops and exits the program
          }
        }
      }

      int minutes = time_left / 60;
      int seconds = time_left % 60;
      mvprintw(0, 0, "Time left: %02d:%02d", minutes, seconds);

      // Move the current piece
      if ((arrow = read_escape(&c)) != NOCHAR)
      {
        int bottom_x = bottom->upper_left_x;
        int bottom_y = bottom->upper_left_y;
        int top_x = top->upper_left_x;
        int top_y = top->upper_left_y;

        switch (arrow)
        {
        // Functionality for Left Arrow Key that controls the bottom slider
        case LEFT:
          moveSlider(bottom, bottom_x - slider_x_speed, bottom_y, zone_width, zone_height);
          mvprintw(1, 5, "L");
          break;
        // Functionality for Rigth Arrow Key that controls the bottom slider
        case RIGHT:
          moveSlider(bottom, bottom_x + slider_x_speed, bottom_y, zone_width, zone_height);
          mvprintw(1, 5, "R");
          break;
        // Functionality for Up Arrow Key that controls the bottom slider
        case UP:
          moveSlider(bottom, bottom_x, bottom_y - slider_y_speed, zone_width, zone_height);
          mvprintw(1, 5, "U");
          break;
        // Functionality for Down Arrow Key that controls the bottom slider
        case DOWN:
          moveSlider(bottom, bottom_x, bottom_y + slider_y_speed, zone_width, zone_height);
          mvprintw(1, 5, "D");
          break;
        // Functionality for A Key that controls the top slider
        case A:
          moveSlider(top, top_x - slider_x_speed, top_y, zone_width, zone_height);
          mvprintw(1, 5, "a");
          break;
          // Functionality for D Key that controls the top slider
        case D:
          moveSlider(top, top_x + slider_x_speed, top_y, zone_width, zone_height);
          mvprintw(1, 5, "d");
          break;
        // Functionality for W Key that controls the top slider
        case W:
          moveSlider(top, top_x, top_y - slider_y_speed, zone_width, zone_height);
          mvprintw(1, 5, "w");
          break;
        // Functionality for S Key that controls the top slider
        case S:
          moveSlider(top, top_x, top_y + slider_y_speed, zone_width, zone_height);
          mvprintw(1, 5, "s");
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
  endwin();
}

// Function to display the end game screen
void end_game_screen(int zone_width, int zone_height)
{
  clear();
  string end_message = "Game Over!";
  string new_game_message = "Press 'N' for a New Game.";
  string exit_message = "Press 'E' to Exit.";

  mvprintw(zone_height / 2 - 1, (zone_width - end_message.length()) / 2, end_message.c_str());
  mvprintw(zone_height / 2, (zone_width - new_game_message.length()) / 2, new_game_message.c_str());
  mvprintw(zone_height / 2 + 1, (zone_width - exit_message.length()) / 2, exit_message.c_str());
  refresh();
}