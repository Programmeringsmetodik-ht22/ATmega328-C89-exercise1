/********************************************************************************
* led_array.h: Innehåller makroliknande funktioner för implementering av
*              dynamiska arrayer innehållande lysdioder i form av pekare till
*              objekt av strukten led.
********************************************************************************/
#ifndef LED_ARRAY_H_
#define LED_ARRAY_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "led.h"

/********************************************************************************
* led_array_new: Allokerar minne för ny dynamisk array av angiven storlek och
*                returnerar en pekare till denna.
*
*                - size: Arrayens storlek, dvs. antalet led-objekt den rymmer.
********************************************************************************/
#define led_array_new(size) (led_t**)malloc(sizeof(led_t*) * size)

/********************************************************************************
* led_array_delete: Tömmer dynamisk array av angiven storlek. Lysdioderna
*                   lagrade i arrayen måste dock nollställas via anrop av
*                   funktionen led_array_clear_content om detta ska ske. 
*                   Adressen till den pekare som pekar på arrayen passeras för
*                   att både kunna frigöra minnet och sätta pekaren till null.
*
*                   - self: Adressen till den pekare som pekar på arrayen.
********************************************************************************/
#define led_array_delete(self) ({ \
   free(*self); \
   *self = 0; \
})

/********************************************************************************
* led_array_clear_content: Nollställer innehållet (lysdioderna) i angiven array.
*                          Arrayen i fråga nollställs dock inte och dess storlek 
*                          ändras inte. För att nollställa arrayen, anropa 
*                          funktionen led_array_delete.
*
*                          - self: Adressen till den pekare som pekar på arrayen.
*                          - size: Arrayens storlek, dvs. antalet lysdioder 
*                                  i arrayen.
********************************************************************************/
#define led_array_clear_content(self, size) ({ \
   struct led_t** i; \
   for (i = 0; i < size; ++i) { \
      led_clear(*i); \
   } \
})

/********************************************************************************
* led_array_resize: Ändrar storlek på befintlig led-array till ny storlek. 
*                   Ifall arrayens storlek lyckas ändras så uppdateras den 
*                   variabel som lagrar arrayens storlek och 0 returneras 0.
*                   Annars returneras felkod 1. 
*
*                   - self    : Pekare till arrayen vars storlek ska ändras.
*                   - new_size: Arrayens nya storlek.
*                   - size    : Pekare till variabel som lagrar aktuell storlek.
********************************************************************************/
#define led_array_resize(self, new_size, size) ({ \
   int ret_val = 0; \
   led_t** copy = (led_t**)realloc(*self, sizeof(led_t*) * new_size); \
   if (!copy) { \
      ret_val = 1; \
   } else { \
      *self = copy; \
      *size = new_size; \
   } \
   ret_val; \
})

/********************************************************************************
* led_array_push: Lägger till ett nytt led-objekt längst bak i angiven array
*                 och räknar upp antalet element efter tillägget.
*                 Vid misslyckad minnesallokering returneras felkod 1.
*                 Annars om push-operationen lyckas returneras 0.
*
*                 - self   : Pekare till array som ska tilldelas.
*                 - size   : Pekare till variabel innehållande arrayens storlek, 
*                            dvs. antalet lysdioder i arrayen.
*                 - new_led: Det nya led-objekt som ska läggas till.
********************************************************************************/
#define led_array_push(self, size, new_led) ({ \
   int ret_val = 0; \
   led_t** copy = (led_t**)realloc(*self, sizeof(led_t*) * (*size + 1)); \
   if (!copy) ret_val = 1; \
   copy[(*size)++] = new_led; \
   *self = copy; \
   ret_val; \
})

/********************************************************************************
* led_array_pop: Tar bort eventuellt sista led-objekt i angiven array genom
*                att minska dess storlek med ett. Vid misslyckad omallokering
*                returneras felkod 1, annars 0.
*
*                - self: Pekare till arrayen vars sista element ska tas bort.
*                - size: Arrayens storlek, dvs. antalet lysdioder i arrayen.
********************************************************************************/
#define led_array_pop(self, size) ({ \
   int ret_val = 0; \
   if ((*size) <= 1) { \
      led_array(*self); \
      (*size) = 0; \
   } else { \
      led_t** copy = (led_t**)realloc(self, sizeof(led_t*) * (*size - 1)); \
      if (!copy) ret_val = 1; \
      else { \
         self = copy; \
         (*size)--; \
      } \
   } \
   ret_val; \
})

/********************************************************************************
* led_array_on: Tänder samtliga lysdioder lagrade i angiven array.
*
*               - self: Pekare till arrayen vars lysdioder ska tändas.
*               - size: Arrayens storlek, dvs. antalet lysdioder i arrayen.
********************************************************************************/
#define led_array_on(self, size) ({ \
   led_t** i; \
   for (i = self; i < self + size; ++i) { \
      (*i)->vptr->on(*i); \
   } \
})

/********************************************************************************
* led_array_off: Släcker samtliga lysdioder lagrade i angiven array.
*
*                - self: Pekare till arrayen vars lysdioder ska släckas.
*                - size: Arrayens storlek, dvs. antalet lysdioder i arrayen.
********************************************************************************/
#define led_array_off(self, size) ({ \
   led_t** i; \
   for (i = self; i < self + size; ++i) { \
      (*i)->vptr->off(*i); \
   } \
})


/********************************************************************************
* led_array_blink_forward: Genomför sekventiell blinkning framåt av samtliga
*                          lysdioder lagrade i angiven array. Därmed blinkar
*                          lysdioderna framåt i en sekvens en efter en.
*
*                          - self          : Pekare till arrayen vars lysdioder
*                                            ska blinkas.
*                          - size          : Arrayens storlek, dvs. antalet
*                                            lysdioder i arrayen.
*                          - blink_speed_ms: Lysdiodernas blinkhastighet mätt
*                                            i millisekunder.
********************************************************************************/
#define led_array_blink_forward(self, size, blink_speed_ms) ({ \
   led_t** i; \
   for (i = self; i < self + size; ++i) { \
      (*i)->vptr->on(*i); \
      delay_ms(blink_speed_ms); \
      (*i)->vptr->off(*i); \
   } \
})

/********************************************************************************
* led_array_blink_backward: Genomför sekventiell blinkning bakåt av samtliga
*                           lysdioder lagrade i angiven array. Därmed blinkar
*                           lysdioderna bakåt i en sekvens en efter en.
*
*                           - self          : Pekare till arrayen vars lysdioder
*                                             ska blinkas.
*                           - size          : Arrayens storlek, dvs. antalet
*                                             lysdioder i arrayen.
*                           - blink_speed_ms: Lysdiodernas blinkhastighet mätt
*                                             i millisekunder.
********************************************************************************/
#define led_array_blink_backward(self, size, blink_speed_ms) ({ \
   led_t** i; \
   for (i = self + size - 1; i >= self; --i) { \
      (*i)->vptr->on(*i); \
      delay_ms(blink_speed_ms); \
      (*i)->vptr->off(*i); \
   } \
})

/********************************************************************************
* led_array_blink_collectively: Genomför kollektiv (synkroniserad) blinkning
*                               av samtliga lysdioder lagrade i angiven array.
*
*                               - self          : Pekare till arrayen vars
*                                                 lysdioder ska blinkas.
*                               - size          : Arrayens storlek, dvs.
*                                                 antalet lysdioder i arrayen.
*                               - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                 mätt i millisekunder.
********************************************************************************/
#define led_array_blink_collectively(self, size, blink_speed_ms) ({ \
   led_array_on(self, size); \
   delay_ms(blink_speed_ms); \
   led_array_off(self, size); \
   delay_ms(blink_speed_ms); \
})

#endif /* LED_ARRAY_H_ */