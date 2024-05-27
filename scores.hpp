#include <vector>

std::vector<int> get_scores();
void save_score(int score);
int calculate_score(int goals, int time);
void write_to_save_file(std::vector<int> scores);