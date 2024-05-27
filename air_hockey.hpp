#include <chrono>
using namespace std;

void air_hockey();
int prompt_slider_size(int zone_height, int zone_width);
int prompt_goal_width(int zone_height, int zone_width);
void end_game_screen(int zone_width, int zone_height, int top_score, int bottom_score, chrono::steady_clock::time_point start_time);