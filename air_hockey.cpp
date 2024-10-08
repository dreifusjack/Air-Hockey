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
#include "scores.hpp"
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <chrono>
#include <vector>
using namespace std;

int top_wins = 0;
int bottom_wins = 0;
int match_length = 0;

// Main Game function
void air_hockey()
{
  int zone_height, zone_width;
  struct timespec tim = {0, 200000000};
  struct timespec tim_ret;
  int arrow, c;
  int slider_x_speed = 4, slider_y_speed = 4;
  initscr();
  start_color();                            // enable colors
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK); // magenta color pair
  init_pair(2, COLOR_RED, COLOR_BLACK);     // red color pair
  init_pair(3, COLOR_BLUE, COLOR_BLACK);    // blue color pair
  init_pair(4, COLOR_GREEN, COLOR_BLACK);   // green color pair
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);  // yellow color pair
  getmaxyx(stdscr, zone_height, zone_width);
  zone_height -= 1;
  zone_width -= 1;

  // get the number of games to be played from the user before starting the game
  int game_count = prompt_game_count(zone_height, zone_width);

  // get the game difficulty from the user before starting the game
  int difficulty = prompt_difficulty_selection(zone_height, zone_width);
  // easy - 1 speed, medium - 2 speed, hard - 3 speed.
  // since the speed has to be an int, and multiplying 1 by 1.2 (20% increase) would lead to 1.2 (a double),
  // it isn't feasible to increase by exactly 20%, so we chose to just use the ceiling of that
  // (1 * 1.2 = 1.2 -> 2, 2 * 1.2 = 2.4 -> 3).
  int ball_speed = difficulty;

  // get the slider size from the user before starting the game
  int slider_size = prompt_slider_size(zone_height, zone_width);

  // get goal size from the user before starting the game
  int goal_width = prompt_goal_width(zone_height, zone_width);

  // Generate obstacles if difficulty is hard
  vector<Obstacle> obstacles;
  if (difficulty == 3)
  {
    obstacles = generate_obstacles(zone_width, zone_height);
  }

  bool game_over = false;
  while (!game_over)
  {
    clear();
    zone_t *z = init_zone(0, 3, zone_width, zone_height - 3, goal_width);
    ball_t *b = init_ball(zone_width / 2, zone_height / 2, ball_speed, ball_speed);
    slider_t *top = init_slider(zone_width / 2, 5, 'T', slider_size);
    slider_t *bottom = init_slider(zone_width / 2, zone_height - 2, 'U', slider_size);
    draw_zone(z);
    draw_slider(top);
    draw_slider(bottom);
    draw_ball(b);
    draw_obstacles(obstacles); // Draw obstacles
    refresh();
    halfdelay(3);
    noecho();
    bool is_paused = false;
    int in_game_input;

    int top_score = 0, bottom_score = 0; // Initialize the scores

    // timer for the game
    auto start_time = chrono::steady_clock::now();
    auto end_time = start_time + chrono::minutes(2);

    bool in_game = true;

    while (in_game)
    {
      refresh();
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
        end_game_screen(zone_width, zone_height, top_score, bottom_score, start_time, game_count);
        game_count--;
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
        draw_obstacles(obstacles); // Redraw obstacles
        refresh();
      }
      // Display the pause and end game instructions at the top of the program
      string pause_instruction = "Press 'P' to pause/resume the game.";
      string end_instruction = "Press 'Q' to end the game.";
      mvprintw(0, (zone_width - pause_instruction.length()) / 2, pause_instruction.c_str());
      mvprintw(1, (zone_width - end_instruction.length()) / 2, end_instruction.c_str());

      auto current_time = chrono::steady_clock::now();
      auto time_left = chrono::duration_cast<chrono::seconds>(end_time - current_time).count();
      if (time_left <= 0)
      {
        in_game = false;
        end_game_screen(zone_width, zone_height, top_score, bottom_score, start_time, game_count);
        game_count--;
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

      // Display the running game score
      mvprintw(2, zone_width - 44, "Top player score: %d | Bottom player score: %d", top_score, bottom_score);

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

      // Check if the ball is in the goal area
      if (b->upper_left_y <= 3 && b->upper_left_x >= (zone_width - goal_width) / 2 + 10 && b->upper_left_x <= (zone_width + goal_width) / 2 + 10)
      {
        bottom_score++;
        refresh();
        undraw_ball(b);
        b = init_ball((zone_width / 2) - ball_speed, (zone_height / 2) - ball_speed, ball_speed, ball_speed); // reset ball to the center
        reset_sliders(top, bottom, zone_width, zone_height);                                                  // reset the sliders to default positions
        refresh();
      }
      else if (b->upper_left_y >= zone_height - 1 && b->upper_left_x >= (zone_width - goal_width) / 2 + 10 && b->upper_left_x <= (zone_width + goal_width) / 2 + 10)
      {
        top_score++;
        refresh();
        undraw_ball(b);
        b = init_ball((zone_width / 2) - ball_speed, (zone_height / 2) - ball_speed, ball_speed, ball_speed); // reset ball to the center
        reset_sliders(top, bottom, zone_width, zone_height);                                                  // reset the sliders to default positions
        refresh();
      }

      refresh();
      undraw_ball(b);
      moveBall(b);
      checkCollisionSlider(bottom, b);
      checkCollisionSlider(top, b);
      checkCollisionWithZone(b, z);
      checkCollisionWithObstacles(b, obstacles);
      undraw_zone(z);
      draw_zone(z);
      draw_ball(b);
      refresh();
      // usleep(200000);
      nanosleep(&tim, &tim_ret);
    }
  }
  endwin();
}

// Prompts the user to enter a slider size for the game
int prompt_slider_size(int zone_height, int zone_width)
{
  int slider_size;
  do
  {
    mvprintw(zone_height / 2, (zone_width - 28) / 2, "Enter a slider size (4-7): ");
    slider_size = getch();
  } while (slider_size < 52 || slider_size > 55); // using ascii values for the numbers 4-7

  // print selection
  mvprintw(zone_height / 2, (zone_width - 28) / 2, "Enter a slider size (4-7): %d", slider_size - 48);

  // convert from ascii value to the correlated value, where 52 = 4, so 52-48 = 4
  return slider_size - 48;
}

// Prompts the user to enter a difficulty for the game
int prompt_difficulty_selection(int zone_height, int zone_width)
{
  int difficulty;
  do
  {
    mvprintw(zone_height / 2 - 1, (zone_width - 51) / 2, "Enter a difficulty (1: Easy, 2: Medium, 3: Hard): ");
    difficulty = getch();
  } while (difficulty < 49 || difficulty > 51); // using ascii values for the numbers 1-3

  // print selection
  mvprintw(zone_height / 2 - 1, (zone_width - 51) / 2, "Enter a difficulty (1: Easy, 2: Medium, 3: Hard): %d", difficulty - 48);

  // convert from ascii value to the correlated value, where 49 = 1, so 49-48 = 1
  return difficulty - 48;
}

// Prompts the user to enter a goal width for the game
int prompt_goal_width(int zone_height, int zone_width)
{
  int goal_width;

  int max_goal_width = zone_width - 10; // max goal width, taking account that the actual zone width is 10 less than inputted to handle padding.
  string goal_width_prompt = "Enter a goal width (must be less than " + to_string(max_goal_width) + ". Press enter for default width): ";

  do
  {
    mvprintw((zone_height / 2) + 1, (zone_width - 75) / 2, goal_width_prompt.c_str());
    echo();
    char input[10];
    getstr(input);
    string input_str(input);
    // handle default input (empty string but user hit enter)
    if (input_str.empty() || (input_str.length() == 1 && input_str[0] == '\n'))
    {
      goal_width = zone_width - 1 - 10; // 1 less than zone width, 10 less to handle padding
    }
    else
    {
      goal_width = stoi(input);
    }
    noecho();
    if (goal_width >= max_goal_width)
    {
      mvprintw((zone_height / 2) + 2, (zone_width - 75) / 2, "Goal width must be less than the zone width. Try again.");
    }
  } while (goal_width >= max_goal_width && goal_width < 0);

  return goal_width;
}

// Prompts the user to enter the ammount of games to be played
int prompt_game_count(int zone_height, int zone_width)
{
  int game_count;
  do
  {
    mvprintw((zone_height / 2) - 2, (zone_width - 47) / 2, "Enter the number of games to play (max of 9): ");
    game_count = getch();
  } while (game_count < 49 || game_count > 57); // using ascii values for the numbers 1-9

  // print selection
  mvprintw((zone_height / 2) - 2, (zone_width - 47) / 2, "Enter the number of games to play (max of 9): %d", game_count - 48);

  // convert from ascii value to the correlated value, where 49 = 1, so 49-48 = 1
  return game_count - 48;
}

// Function to display the end game screen
void end_game_screen(int zone_width, int zone_height, int top_goals, int bottom_goals, chrono::steady_clock::time_point start_time, int game_count)
{
  string round_winner;
  if (top_goals > bottom_goals)
  {
    round_winner = "Top Player Wins the Round!";
    top_wins++;
  }
  else if (bottom_goals > top_goals)
  {
    round_winner = "Bottom Player Wins the Round!";
    bottom_wins++;
  }
  else
  {
    round_winner = "It's a Draw!";
  }

  // Calculate time taken
  auto end_time = chrono::steady_clock::now();
  auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
  match_length += duration;

  // Try saving the score to see if it is in the top 10 for the top and bottom player
  int top_score = calculate_score(top_goals, duration);
  int bottom_score = calculate_score(bottom_goals, duration);
  save_score(top_score);
  save_score(bottom_score);

  if (game_count == 1)
  {
    clear();
    string end_message = "Game Over!";
    string exit_message = "Press 'E' to Exit.";
    string top_wins_message = "Top Player Wins: " + to_string(top_wins);
    string botton_wins_message = "Bottom Player Wins: " + to_string(bottom_wins);

    string winner;
    if (top_wins > bottom_wins)
    {
      winner = "Top Player Wins the Game!";
    }
    else if (bottom_wins > top_wins)
    {
      winner = "Bottom Player Wins the Game!";
    }
    else
    {
      winner = "It's a Draw!";
    }

    // total time taken
    int minutes = match_length / 60;
    int seconds = match_length % 60;
    string time_taken = "Time Taken: " + to_string(minutes) + "m " + to_string(seconds) + "s";

    // Display the end of game info
    mvprintw(zone_height / 2 - 2, (zone_width - end_message.length()) / 2, end_message.c_str());
    mvprintw(zone_height / 2 - 1, (zone_width - winner.length()) / 2, winner.c_str());
    mvprintw(zone_height / 2, (zone_width - time_taken.length()) / 2, time_taken.c_str());
    mvprintw(zone_height / 2 + 1, (zone_width - top_wins_message.length()) / 2, top_wins_message.c_str());
    mvprintw(zone_height / 2 + 2, (zone_width - botton_wins_message.length()) / 2, botton_wins_message.c_str());
    mvprintw(zone_height / 2 + 3, (zone_width - exit_message.length()) / 2, exit_message.c_str());
    refresh();
  }
  else
  {
    clear();
    string round_message = "Round Over!";
    string new_round_message = "Press 'N' for the next Round.";

    // round time taken
    int minutes = duration / 60;
    int seconds = duration % 60;
    string time_taken = "Time Taken: " + to_string(minutes) + "m " + to_string(seconds) + "s";

    // Display the end of round info
    mvprintw(zone_height / 2 - 2, (zone_width - round_message.length()) / 2, round_message.c_str());
    mvprintw(zone_height / 2 - 1, (zone_width - round_winner.length()) / 2, round_winner.c_str());
    mvprintw(zone_height / 2, (zone_width - time_taken.length()) / 2, time_taken.c_str());
    mvprintw(zone_height / 2 + 1, (zone_width - 44) / 2, "Top player goals: %d | Top player score: %d", top_goals, top_score);
    mvprintw(zone_height / 2 + 2, (zone_width - 50) / 2, "Bottom player goals: %d | Bottom player score: %d", bottom_goals, bottom_score);
    mvprintw(zone_height / 2 + 3, (zone_width - new_round_message.length()) / 2, new_round_message.c_str());
    refresh();
  }
  // Display the current top 10 scores
  vector<int> scores = get_scores();
  mvprintw(2, 2, "Leaderboard (scores calculated goals/seconds * 1000):");
  for (size_t i = 0; i < scores.size(); i++)
  {
    mvprintw(3 + i, 2, "%zu: %d", i + 1, scores[i]);
  }
}

// Function to generate random obstacle positions
vector<Obstacle> generate_obstacles(int zone_width, int zone_height)
{
  vector<Obstacle> obstacles;
  // create two fixed point obstacles, relative to zone width and height
  for (int i = 0; i < 2; ++i)
  {
    if (i == 0)
    {
      int x = zone_width / 2 + 4;
      int y = zone_height / 2;
      obstacles.push_back({x, y});
    }
    else
    {
      int x = zone_width / 3 - 13;
      int y = zone_height / 3;
      obstacles.push_back({x, y});
    }
  }
  return obstacles;
}

// Function to draw obstacles
void draw_obstacles(const vector<Obstacle> &obstacles)
{
  for (const auto &obs : obstacles)
  {
    attron(COLOR_PAIR(5)); // add colors
    mvprintw(obs.y, obs.x, "#####");
  }
  attroff(COLOR_PAIR(5)); // reset colors
}
