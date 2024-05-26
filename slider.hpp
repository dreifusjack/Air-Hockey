// Properties of the slider
typedef struct slider
{
  char type_str[7];
  char piece[1][7]; // increase to 7, which is the max size
  int upper_left_x;
  int upper_left_y;
  char draw_char;
  int color_pair;
  int size;
} slider_t;

slider_t *init_slider(int initial_x, int initial_y, char type, int size);
void draw_slider(slider_t *s);
void undraw_slider(slider_t *s);
void moveSlider(slider_t *s, int x, int y, int zone_width, int zone_height);