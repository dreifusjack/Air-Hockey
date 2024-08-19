# Air Hockey Game
## Overview
This is a terminal-based air hockey game built using C++ and the ncurses library. The game features two-player action, where each player controls a mallet to hit the puck and score goals. The game follows traditional air hockey rules, where a goal is counted only if the puck fully crosses the goal line.

### Game Controls
#### Player 1 (Top mallet):
- W: Move up
- S: Move down
- A: Move left
- D: Move right
  
##### Player 2 (Bottom mallet):
- Arrow Up: Move up
- Arrow Down: Move down
- Arrow Left: Move left
- Arrow Right: Move right
### Game Rules
Score goals by hitting the puck into the opponent's goal area.
A goal is only counted if the puck fully drops into the goal.
The game keeps track of each player's score and displays it at the top of the screen.
### How to Run the Game
#### Prerequisites
Make sure you have the following installed on your system:
- g++ compiler
- ncurses library
#### Compiling the Game
- Clone the repository or download the source code.
- Open a terminal and navigate to the directory containing the source files.
- Run the following command to compile the game: "make". This will compile the game and create an executable file.

#### Running the Game
Once the game is compiled, you can start it by running: "./air_hockey". Enjoy!

##### Cleaning Up
To clean up the compiled files, you can run: "make clean". This will remove all object files and the executable.
