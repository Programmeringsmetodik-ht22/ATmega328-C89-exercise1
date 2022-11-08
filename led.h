/********************************************************************************
* led.h: Inneh�ller funktionalitet f�r enkel implementering av lysdioder via
*        strukten led samt associerade funktioner, som n�s via en vtable-pekare. 
*        Denna strukt fungerar ocks� utm�rkt f�r andra digitala utportar.
********************************************************************************/
#ifndef LED_H_
#define LED_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

struct led_vtable; /* F�rdeklarerar inf�r deklaration av strukten led. */

/********************************************************************************
* led: Strukt f�r implementering av lysdioder och andra digitala utportar.
********************************************************************************/
typedef struct led
{
   uint8_t pin;             /* Lysdiodens pin-nummer p� aktuell I/O-port. */
   enum io_port io_port;    /* I/O-port som lysdioden �r ansluten till. */
   bool enabled;            /* Indikerar ifall lysdioden �r t�nd. */
   struct led_vtable* vptr; /* Pekare till vtable inneh�llande associerade funktioner. */
} led_t, *led_ptr_t;

/********************************************************************************
* led_vtable: Strukt f�r vtable inneh�llande funktionspekare till associerade
*             funktioner f�r strukten led.
********************************************************************************/
typedef struct led_vtable
{
   /********************************************************************************
   * on: T�nder lysdioden.
   *
   *     - self: Pekare till lysdioden som ska t�ndas.
   ********************************************************************************/
   void (*on)(led_t* self); 

   /********************************************************************************
   * off: Sl�cker lysdioden.
   *
   *     - self: Pekare till lysdioden som ska sl�ckas.
   ********************************************************************************/                   
   void (*off)(led_t* self);
   
   /********************************************************************************
   * toggle: Togglar utsignalen p� lysdioder fr�n t�nd till sl�ckt eller tv�rtom.
   *
   *         - self: Pekare till lysdioden vars utsignal ska togglas.
   ********************************************************************************/                    
   void (*toggle)(led_t* self);  
   
   /********************************************************************************
   * blink: Blinkar lysdioden en g�ng med angiven blinkhastighet. F�r kontinuerlig
   *        blinkning m�ste denna funktion anropas i en fortg�ende loop.
   *
   *        - self: Pekare till lysdioden som ska blinkas.
   ********************************************************************************/                  
   void (*blink)(led_t* self, const uint16_t blink_speed_ms); 

} *led_vptr_t;


/********************************************************************************
* led_init: Initierar ny lysdiod p� angiven pin.
*
*           - self: Pekare till lysdioden som ska initieras.
*           - pin : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8. 
*                   Alternativt kan motsvarande port-nummer p� ATmega328P 
*                   anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
void led_init(led_t* self, 
              const uint8_t pin);

/********************************************************************************
* led_clear: Nollst�ller lysdiod samt motsvarande pin.
*
*            - self: Pekare till lysdioden som ska nollst�llas.
********************************************************************************/
void led_clear(led_t* self);

/********************************************************************************
* led_new: Allokerar minne och initierar en ny lysdiod p� angiven pin.
*          En pekare returneras till lysdioden efter initieringen.
*          Vid misslyckad minnesallokering returneras en nullpekare.
*
*          - pin : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
*                  Alternativt kan motsvarande port-nummer p� ATmega328P
*                  anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
led_t* led_new(const uint8_t pin);

/********************************************************************************
* led_delete: Frig�r minne allokerat f�r angiven lysdiod och s�tter motsvarande
*             pekare till null. F�r att kunna genomf�ra detta passeras adressen
*             till den pekare som pekar p� lysdioden.
*
*             - self: Adressen till den pekare som pekar p� lysdioden.
********************************************************************************/
void led_delete(led_t** self);

#endif /* LED_H_ */