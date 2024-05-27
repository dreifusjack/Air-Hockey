#include "scores.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * Gets the current top 10 scores that have been saved.
 */
vector<int> get_scores()
{
  ifstream infile("./saves/save_best_10.game");
  if (!infile)
  {
    return {};
  }
  else
  {
    vector<int> scores;
    string line;

    // read each line from the input
    while (getline(infile, line))
    {
      scores.push_back(stoi(line));
    }

    return scores;
  }
}

/**
 * Attempts to save a given score. If it is in the top 10, it will be saved. If it is
 * not in the top 10, it will not be saved.
 * The score is calculated by the number of goals divided by the amount of time taken,
 * multiplied by 1000, and the floor of that value.
 */
void save_score(int score)
{
  // Check if it is in the top 10
  vector<int> current_scores = get_scores();
  bool new_top_10 = false;
  for (int existing_score : current_scores)
  {
    if (score > existing_score)
      new_top_10 = true;
  }

  // Save the score if there aren't 10 scores yet
  if (current_scores.size() < 10)
  {
    current_scores.push_back(score);
  }
  // If there are already top 10, but this score replaces at least one of them
  else if (new_top_10)
  {
    // Replace the lowest score
    int min_score_index = 0;
    for (size_t i = 1; i < current_scores.size(); i++)
    {
      if (current_scores[i] < current_scores[min_score_index])
        min_score_index = i;
    }
    current_scores[min_score_index] = score;
  }

  // Sort the scores by highest to lowest
  sort(current_scores.rbegin(), current_scores.rend());

  write_to_save_file(current_scores);
}

/**
 * Calculates a player's score from their goals and time.
 */
int calculate_score(int goals, int time)
{
  double raw_score = (static_cast<double>(goals) / time) * 1000;
  return max(0, static_cast<int>(floor(raw_score)));
}

/**
 * Writes given scores to the save file.
 */
void write_to_save_file(std::vector<int> scores)
{
  ofstream outfile("./saves/save_best_10.game");
  if (outfile)
  {
    for (int score : scores)
      outfile << to_string(score) << endl;
  }
}