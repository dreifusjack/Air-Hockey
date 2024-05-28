/* key.cpp ---
 *
 * Filename: key.cpp
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

/* Code: */
#include "key.hpp"
#include "air_hockey.hpp"
#include <ncurses.h>
#include <string>
using namespace std;

int read_escape(int *read_char)
{
  int c;
  if ((c = getch()) == ERR)
  {
    return (NOCHAR);
  }
  else if (c == 0x1b)
  {
    if ((c = getch()) == '[')
    {
      c = getch();
      if (c == 'a')
        return (A);
      else if (c == 's')
        return (S);
      else if (c == 'd')
        return (D);
      else if (c == 'w')
        return (W);

      switch (c)
      {
      case 'A':
        return (UP);
        break;
      case 'B':
        return (DOWN);
        break;
      case 'C':
        return (RIGHT);
        break;
      case 'D':
        return (LEFT);
        break;
      default:
        return (BADESC);
      }
    }
  }
  else
  {
    *read_char = c;
    switch (c)
    {
    case 'a':
    case 'A':
      return A;

    case 'w':
    case 'W':
      return W;

    case 's':
    case 'S':
      return S;

    case 'd':
    case 'D':
      return D;

    default:
      return (REGCHAR);
    }
  }
  return (REGCHAR);
}

void welcome_page()
{
  initscr();   // Initialize the window
  cbreak();    // Disable line buffering
  noecho();    // Disable echoing of typed characters
  curs_set(0); // Hide the cursor

  int zone_height, zone_width;
  getmaxyx(stdscr, zone_height, zone_width);

  // Centering the welcome message
  string welcome_message = "Welcome to Air Hockey!";
  mvprintw(zone_height / 2 - 7, (zone_width - welcome_message.length()) / 2, welcome_message.c_str());

  // Centering the scoring summary title
  string scoring_title = "Scoring System:";
  mvprintw(zone_height / 2 - 3, (zone_width - scoring_title.length()) / 2, scoring_title.c_str());

  // Centering the scoring summary list
  string scoring_summary[] = {
      "1. Two players each control a mallet.",
      "2. Strike the puck with your mallet to score goals.",
      "3. A goal is counted only if the puck fully drops in the goal area.",
      "4. Rebounds or pucks halfway in the goal area do not count.",
      "5. See the goals at the top of the screen.",
      "6. The final score is calculated based on the number of goals scored in a time."};

  for (int i = 0; i < 6; ++i)
  {
    mvprintw(zone_height / 2 - 2 + i, (zone_width - scoring_summary[i].length()) / 2, scoring_summary[i].c_str());
  }

  // Press 't' to start
  string start_message = "Press 't' to start the game!";
  mvprintw(zone_height / 2 - 5, (zone_width - start_message.length()) / 2, start_message.c_str());

  // Refresh the window to show the changes
  refresh();

  // Wait for user input
  int ch;
  while ((ch = getch()) != 't')
  {
    // Wait until 't' is pressed
  }
  clear();      // Clear the screen
  refresh();    // Refresh to show the cleared screen
  endwin();     // End ncurses mode
  air_hockey(); // Call air_hockey function
}

/* key.cpp ends here */
