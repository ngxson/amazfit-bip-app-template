/*
  Application template for Amazfit Bip BipOS
  (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

  Template application, header file
*/

#ifndef __APP_TEMPLATE_H__
#define __APP_TEMPLATE_H__

#define COLORS_COUNT 4

// Data structure for our screen
struct app_data_
{
  void *ret_f; //  address of the return function
  int col;     //  current color of the text
};

// template.c
void show_screen(void *return_screen);
void key_press_screen();
int dispatch_screen(void *param);
void screen_job();
void draw_screen(int col);

#endif
