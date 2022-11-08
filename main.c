/********************************************************************************
* main.c: Demonstration av funktionsliknande makron, struktar med vtablepekare 
*         samt dynamisk minnesallokering. Programkod skriven i C89.
********************************************************************************/
#include "led.h"
#include "button.h"
#include "led_array.h"

/********************************************************************************
* num_buttons_pressed: Returnerar antalet nedtryckta tryckknappar.
*
*                      - button1: Pekare till den första tryckknappen.
*                      - button2: Pekare till den andra tryckknappen.  
*                      - button3: Pekare till den tredje tryckknappen.       
*                      - button4: Pekare till den fjärde tryckknappen.   
********************************************************************************/
static uint8_t num_buttons_pressed(button_t* button1,
                                   button_t* button2,
                                   button_t* button3,
                                   button_t* button4)
{
   uint8_t num = 0;
   if (button1->vptr->is_pressed(button1)) num++;
   if (button2->vptr->is_pressed(button2)) num++;
   if (button3->vptr->is_pressed(button3)) num++;
   if (button4->vptr->is_pressed(button4)) num++;
   return num;
}

/********************************************************************************
* main: Ansluter fem lysdioder till pin 6 - 10 samt fyra tryckknappar till pin  
*       11 - 13 samt pin 2. Lysdioderna lagras i en dynamisk array. 
*       Beroende på antalet tryckknappar som trycks ned så blinkar lysdioderna 
*       antingen framåt, bakåt eller synkroniserat, eller så hålls de tända 
*       eller släckta. 
********************************************************************************/
int main(void)
{
   led_t* led1 = led_new(6);
   led_t* led2 = led_new(7);
   led_t* led3 = led_new(8);
   led_t* led4 = led_new(9);
   led_t* led5 = led_new(10);

   button_t* button1 = button_new(11);
   button_t* button2 = button_new(12);
   button_t* button3 = button_new(13);
   button_t* button4 = button_new(2);

   led_t** leds = led_array_new(0);
   size_t num_leds = 0;

   if (!leds) return 1;

   led_array_push(&leds, &num_leds, led1);
   led_array_push(&leds, &num_leds, led2);
   led_array_push(&leds, &num_leds, led3);
   led_array_push(&leds, &num_leds, led4);
   led_array_push(&leds, &num_leds, led5);

   while (1)
   {
      const uint8_t buttons_pressed = num_buttons_pressed(button1, button2, button3, button4);

      if (buttons_pressed == 0)
      {
         led_array_off(leds, num_leds);
      }
      else if (buttons_pressed == 1)
      {
         led_array_blink_collectively(leds, num_leds, 100);
      }
      else if (buttons_pressed == 2)
      {
         led_array_blink_forward(leds, num_leds, 100);
      }
      else if (buttons_pressed == 3)
      {
         led_array_blink_backward(leds, num_leds, 100);
      }
      else if (buttons_pressed == 4)
      {
          led_array_on(leds, num_leds);
      }
      else
      {
         led_array_off(leds, num_leds);
      }
   }

   return 0;
}

