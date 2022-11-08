/********************************************************************************
* led_array.h: Inneh�ller makroliknande funktioner f�r implementering av
*              dynamiska arrayer inneh�llande lysdioder i form av pekare till
*              objekt av strukten led.
********************************************************************************/
#ifndef LED_ARRAY_H_
#define LED_ARRAY_H_

/* Inkluderingsdirektiv: */
#include "misc.h"
#include "led.h"

/********************************************************************************
* led_array_new: Allokerar minne f�r ny dynamisk array av angiven storlek och
*                returnerar en pekare till denna.
*
*                - size: Arrayens storlek, dvs. antalet led-objekt den rymmer.
********************************************************************************/
#define led_array_new(size) (led_t**)malloc(sizeof(led_t*) * size)

/********************************************************************************
* led_array_delete: T�mmer dynamisk array av angiven storlek. Lysdioderna
*                   lagrade i arrayen m�ste dock nollst�llas via anrop av
*                   funktionen led_array_clear_content om detta ska ske. 
*                   Adressen till den pekare som pekar p� arrayen passeras f�r
*                   att b�de kunna frig�ra minnet och s�tta pekaren till null.
*
*                   - self: Adressen till den pekare som pekar p� arrayen.
********************************************************************************/
#define led_array_delete(self) ({ \
   free(*self); \
   *self = 0; \
})

/********************************************************************************
* led_array_clear_content: Nollst�ller inneh�llet (lysdioderna) i angiven array.
*                          Arrayen i fr�ga nollst�lls dock inte och dess storlek 
*                          �ndras inte. F�r att nollst�lla arrayen, anropa 
*                          funktionen led_array_delete.
*
*                          - self: Adressen till den pekare som pekar p� arrayen.
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
* led_array_resize: �ndrar storlek p� befintlig led-array till ny storlek. 
*                   Ifall arrayens storlek lyckas �ndras s� uppdateras den 
*                   variabel som lagrar arrayens storlek och 0 returneras 0.
*                   Annars returneras felkod 1. 
*
*                   - self    : Pekare till arrayen vars storlek ska �ndras.
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
* led_array_push: L�gger till ett nytt led-objekt l�ngst bak i angiven array
*                 och r�knar upp antalet element efter till�gget.
*                 Vid misslyckad minnesallokering returneras felkod 1.
*                 Annars om push-operationen lyckas returneras 0.
*
*                 - self   : Pekare till array som ska tilldelas.
*                 - size   : Pekare till variabel inneh�llande arrayens storlek, 
*                            dvs. antalet lysdioder i arrayen.
*                 - new_led: Det nya led-objekt som ska l�ggas till.
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
* led_array_on: T�nder samtliga lysdioder lagrade i angiven array.
*
*               - self: Pekare till arrayen vars lysdioder ska t�ndas.
*               - size: Arrayens storlek, dvs. antalet lysdioder i arrayen.
********************************************************************************/
#define led_array_on(self, size) ({ \
   led_t** i; \
   for (i = self; i < self + size; ++i) { \
      (*i)->vptr->on(*i); \
   } \
})

/********************************************************************************
* led_array_off: Sl�cker samtliga lysdioder lagrade i angiven array.
*
*                - self: Pekare till arrayen vars lysdioder ska sl�ckas.
*                - size: Arrayens storlek, dvs. antalet lysdioder i arrayen.
********************************************************************************/
#define led_array_off(self, size) ({ \
   led_t** i; \
   for (i = self; i < self + size; ++i) { \
      (*i)->vptr->off(*i); \
   } \
})


/********************************************************************************
* led_array_blink_forward: Genomf�r sekventiell blinkning fram�t av samtliga
*                          lysdioder lagrade i angiven array. D�rmed blinkar
*                          lysdioderna fram�t i en sekvens en efter en.
*
*                          - self          : Pekare till arrayen vars lysdioder
*                                            ska blinkas.
*                          - size          : Arrayens storlek, dvs. antalet
*                                            lysdioder i arrayen.
*                          - blink_speed_ms: Lysdiodernas blinkhastighet m�tt
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
* led_array_blink_backward: Genomf�r sekventiell blinkning bak�t av samtliga
*                           lysdioder lagrade i angiven array. D�rmed blinkar
*                           lysdioderna bak�t i en sekvens en efter en.
*
*                           - self          : Pekare till arrayen vars lysdioder
*                                             ska blinkas.
*                           - size          : Arrayens storlek, dvs. antalet
*                                             lysdioder i arrayen.
*                           - blink_speed_ms: Lysdiodernas blinkhastighet m�tt
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
* led_array_blink_collectively: Genomf�r kollektiv (synkroniserad) blinkning
*                               av samtliga lysdioder lagrade i angiven array.
*
*                               - self          : Pekare till arrayen vars
*                                                 lysdioder ska blinkas.
*                               - size          : Arrayens storlek, dvs.
*                                                 antalet lysdioder i arrayen.
*                               - blink_speed_ms: Lysdiodernas blinkhastighet
*                                                 m�tt i millisekunder.
********************************************************************************/
#define led_array_blink_collectively(self, size, blink_speed_ms) ({ \
   led_array_on(self, size); \
   delay_ms(blink_speed_ms); \
   led_array_off(self, size); \
   delay_ms(blink_speed_ms); \
})

#endif /* LED_ARRAY_H_ */