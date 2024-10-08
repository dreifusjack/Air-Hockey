/* ball.cpp ---
 *
 * Filename: ball.cpp
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

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdbool>
#include <cmath>
#include <ncurses.h>
#include "key.hpp"
#include "ball.hpp"

// Move the ball based on its speed
void moveBall(ball_t *b)
{
  b->upper_left_x += b->speed_x;
  b->upper_left_y += b->speed_y;
}

// Check if the ball collides with the slider.
// Change Y direction of the ball if it collides
bool checkCollisionSlider(slider_t *s, ball_t *b)
{
  int x_ball = b->upper_left_x;
  int y_ball = b->upper_left_y;
  int x, y;
  for (x = 0; x < s->size; x++)
  {
    for (y = 0; y < 1; y++)
    {
      // if the x-pos match
      if (s->upper_left_x + x == x_ball)
      {
        // if y-pos match exactly
        if (s->upper_left_y + y == y_ball)
        {
          b->speed_y *= -1;
          return true;
        }
        // if the speed is 2 or 3, and the ball is within 1 of the slider. this is necessary
        // because the ball may "jump" over the slider if the y-pos doesn't match exactly
        if ((b->speed_y == 2 || b->speed_y == 3) && abs(s->upper_left_y + y - y_ball) == 1)
        {
          b->speed_y *= -1;
          return true;
        }
        // same thing if the speed is 3, and the ball is within 2 of the slider, it will rebound
        // after the next frame, since without this check the ball pos would be 1 below the slider
        // and technically never "collided"
        if (b->speed_y == 3 && abs(s->upper_left_y + y - y_ball) == 2)
        {
          b->speed_y *= -1;
          return true;
        }
      }
    }
  }
  draw_slider(s);
  return false;
}

// Function to check ball collision with obstacles
bool checkCollisionWithObstacles(ball_t *b, const vector<Obstacle> &obstacles)
{
  int y_ball = b->upper_left_y;
  int x_ball = b->upper_left_x;
  for (const auto &obs : obstacles)
  {
    // obstacle is 5 wide
    for (int x = 0; x < 5; x++)
    {
      for (int y = 0; y < 1; y++)
      {
        // if x-pos matches
        if (obs.x + x == x_ball)
        {
          // if y-pos matches exactly
          if (obs.y + y == y_ball)
          {
            b->speed_y *= -1;
            return true;
          }
          // if the speed is 2 or 3 and the ball is within 1 of the obstacle (need since there
          // is a chance the ball "jumps" over the obstacle)
          if ((b->speed_y == 2 || b->speed_y == 3) && abs(obs.y + y - y_ball) == 1)
          {
            b->speed_y *= -1;
            return true;
          }
          // if speed 3
          if (b->speed_y == 3 && abs(obs.y + y - y_ball) == 2)
          {
            b->speed_y *= -1;
            return true;
          }
        }
      }
    }
  }
  draw_obstacles(obstacles);
  return false;
}

// Check if the ball collides with the left walls of the zone.
// Change X direction of the ball if it collides
bool checkCollisionWithZone(ball_t *b, zone_t *z)
{
  bool ball_has_collided = false;

  // check left, right zones
  if (b->upper_left_x <= z->upper_left_x || b->upper_left_x >= z->upper_left_x + z->width)
  {
    // Check if the ball is hitting the left or right wall of the zone
    b->speed_x *= -1;
    ball_has_collided = true;
  }

  // check top, bottom zones (not including goal)
  int goal_start_x = z->upper_left_x + ((z->width - z->goal_width) / 2) + 1;
  bool ball_collides_y_zone = b->upper_left_x < goal_start_x || b->upper_left_x > goal_start_x + z->goal_width;
  if (ball_collides_y_zone && (b->upper_left_y <= z->upper_left_y || b->upper_left_y >= z->upper_left_y + z->height))
  {
    b->speed_y *= -1;
    ball_has_collided = true;
  }

  return ball_has_collided;
}

// Inititialize ball with its position and speed in the X & Y directions
ball_t *init_ball(int upper_left_x, int upper_left_y, int speed_x, int speed_y)
{
  ball_t *b;
  b = (ball_t *)malloc(sizeof(ball_t));
  b->draw_char = 'O';
  b->upper_left_x = upper_left_x;
  b->upper_left_y = upper_left_y;
  b->color[0] = 0;
  b->color[1] = 0;
  b->color[2] = 0;
  b->speed_x = speed_x;
  b->speed_y = speed_y;
  // printf("%c", b->draw_char);
  return (b);
}

// Renders the ball on the screen
void draw_ball(ball_t *b)
{
  mvprintw(b->upper_left_y, b->upper_left_x, "%c", b->draw_char);
}

// Replaces the ball with blank space
void undraw_ball(ball_t *b)
{
  mvprintw(b->upper_left_y, b->upper_left_x, " ", b->draw_char);
}
