/*
  Application template for Amazfit Bip BipOS
  (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

  Template application for the BipOS bootloader
*/

#include <libbip.h>
#include "hello_world.h"

//  Screen menu structure - each screen has its own
struct regmenu_ screen_data = {
    55,               //  main screen number, value 0-255, for user windows it is better to take 50 and above
    1,                //  auxiliary screen number (usually equal to 1)
    0,                //  0
    dispatch_screen,  //  pointer to the function for handling touch, swipes, long button press
    key_press_screen, //  pointer to the function for handling button press
    screen_job,       //  pointer to the timer callback function
    0,                //  0
    show_screen,      //  pointer to the function for displaying the screen
    0,                //
    0                 //  long button press
};

int main(int param0, char **argv)
{ //  here the variable argv is not defined
  show_screen((void *)param0);
}

void show_screen(void *param0)
{
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to the screen data
  struct app_data_ *app_data;                           //  pointer to the screen data

  Elf_proc_ *proc;

  // check the source of the procedure launch
  if ((param0 == *app_data_p) && get_var_menu_overlay())
  { // return from the overlay screen (incoming call, notification, alarm, goal, etc.)

    app_data = *app_data_p; //  the pointer to the data must be saved to avoid
                            //  freeing memory by the reg_menu function
    *app_data_p = NULL;     //  nullify the pointer for passing to the reg_menu function

    //   create a new screen, with temp_buf_2 pointer being 0 and memory not freed
    reg_menu(&screen_data, 0); //   menu_overlay=0

    *app_data_p = app_data; //  restore the data pointer after the reg_menu function

    //   here we perform actions when returning from the overlay screen: restore data, etc.
  }
  else
  { // if the function is launched for the first time, i.e., from the menu

    // create a screen (register in the system)
    reg_menu(&screen_data, 0);

    // allocate the necessary memory and place the data in it (memory at the pointer stored at temp_buf_2 address is automatically freed by the reg_menu function of another screen)
    *app_data_p = (struct app_data_ *)pvPortMalloc(sizeof(struct app_data_));
    app_data = *app_data_p; //  pointer to the data

    // clear the memory for data
    _memclr(app_data, sizeof(struct app_data_));

    //  the value param0 contains a pointer to the data of the running process structure Elf_proc_
    proc = param0;

    // remember the address of the function pointer to which we need to return after completing this screen
    if (param0 && proc->ret_f) //  if the return pointer is passed, return to it
      app_data->ret_f = proc->elf_finish;
    else //  if not, return to the watchface
      app_data->ret_f = show_watchface;

    // here we perform actions necessary if the function is launched for the first time from the menu: fill all data structures, etc.

    app_data->col = 0;
  }

  // here we perform interface drawing, no need to update (transfer to video memory) the screen

  draw_screen(app_data->col);

  // if necessary, set the timer for calling screen_job in ms
  set_update_period(1, 5000);
}

void key_press_screen()
{
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to the screen data
  struct app_data_ *app_data = *app_data_p;             //  pointer to the screen data

  // call the return function (usually this is the launch menu), specify the address of our application function as a parameter
  show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void screen_job()
{
  // if necessary, you can use screen data in this function
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to the screen data
  struct app_data_ *app_data = *app_data_p;             //  pointer to the screen data

  // perform periodic action: animation, counter increase, screen update,
  // the interface drawing, updating (transfer to video memory) the screen needs to be done

  app_data->col = (app_data->col + 1) % COLORS_COUNT;
  draw_screen(app_data->col);

  // transfer the lines of the screen that were redrawn to video memory
  repaint_screen_lines(0, 176);

  // if necessary, set the timer for calling screen_job again
  set_update_period(1, 5000);
}

int dispatch_screen(void *param)
{
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to the screen data
  struct app_data_ *app_data = *app_data_p;             //  pointer to the screen data

  // in case of interface drawing, updating (transfer to video memory) the screen needs to be done

  struct gesture_ *gest = param;
  int result = 0;

  switch (gest->gesture)
  {
  case GESTURE_CLICK:
  {
    if ((gest->touch_pos_y > 66) && (gest->touch_pos_y < 110))
    {
      if ((gest->touch_pos_x > 66) && (gest->touch_pos_x < 110))
      {
        // touch in the center of the screen
        // perform actions
        vibrate(1, 70, 0);
        app_data->col = (app_data->col + 1) % COLORS_COUNT;
        draw_screen(app_data->col);
        repaint_screen_lines(0, 176);
      }
    }
    break;
  };
  case GESTURE_SWIPE_RIGHT:
  { //  swipe right
    // usually this is an exit from the application
    show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
    break;
  };
  case GESTURE_SWIPE_LEFT:
  { // swipe left
    // actions on swipe left
    break;
  };
  case GESTURE_SWIPE_UP:
  { // swipe up
    // actions on swipe up
    break;
  };
  case GESTURE_SWIPE_DOWN:
  { // swipe down
    // actions on swipe down
    break;
  };
  default:
  { // something went wrong...

    break;
  };
  }

  return result;
};

// custom function
void draw_screen(int col)
{

  static int colors_bg[COLORS_COUNT] = {COLOR_BLACK, COLOR_BLUE, COLOR_RED, COLOR_PURPLE};
  static int colors_fg[COLORS_COUNT] = {COLOR_AQUA, COLOR_WHITE, COLOR_YELLOW, COLOR_GREEN};

  set_bg_color(colors_bg[col]);
  fill_screen_bg();
  set_graph_callback_to_ram_1();
  // load fonts
  load_font();
  set_fg_color(colors_fg[col]);

  text_out_center("Hello!!!", 88, 60);
};
