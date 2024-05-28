#ifndef AIR_HOCKEY_HPP
#define AIR_HOCKEY_HPP

#include <chrono>
using namespace std;

void air_hockey();
int prompt_slider_size(int zone_height, int zone_width);
int prompt_difficulty_selection(int zone_height, int zone_width);
int prompt_goal_width(int zone_height, int zone_width);
int prompt_game_count(int zone_height, int zone_width);
void end_game_screen(int zone_width, int zone_height, int top_score, int bottom_score, chrono::steady_clock::time_point start_time, int game_count);

struct Obstacle
{
    int x;
    int y;
};
vector<Obstacle> generate_obstacles(int zone_width, int zone_height);
void draw_obstacles(const vector<Obstacle> &obstacles);

#endif