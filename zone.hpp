// This structure holds the properties of the zone such as position and dimesions
typedef struct zone
{
  int upper_left_x;
  int upper_left_y;
  int width;
  int height;
  char draw_char;
  int zone_color;
  char goal_char;
  int goal_color;
  int goal_width;
} zone_t;

zone_t *init_zone(int upper_left_x, int upper_left_y, int width, int height, int goal_width);
void undraw_zone(zone_t *z);
void draw_zone(zone_t *z);
