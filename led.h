/********************************************************************************
* led.h: Innehåller funktionalitet för enkel implementering av lysdioder via
*        strukten led samt associerade funktioner, som nås via en vtable-pekare. 
*        Denna strukt fungerar också utmärkt för andra digitala utportar.
********************************************************************************/
#ifndef LED_H_
#define LED_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

struct led_vtable; /* Fördeklarerar inför deklaration av strukten led. */

/********************************************************************************
* led: Strukt för implementering av lysdioder och andra digitala utportar.
********************************************************************************/
typedef struct led
{
   uint8_t pin;             /* Lysdiodens pin-nummer på aktuell I/O-port. */
   enum io_port io_port;    /* I/O-port som lysdioden är ansluten till. */
   bool enabled;            /* Indikerar ifall lysdioden är tänd. */
   struct led_vtable* vptr; /* Pekare till vtable innehållande associerade funktioner. */
} led_t, *led_ptr_t;

/********************************************************************************
* led_vtable: Strukt för vtable innehållande funktionspekare till associerade
*             funktioner för strukten led.
********************************************************************************/
typedef struct led_vtable
{
   /********************************************************************************
   * on: Tänder lysdioden.
   *
   *     - self: Pekare till lysdioden som ska tändas.
   ********************************************************************************/
   void (*on)(led_t* self); 

   /********************************************************************************
   * off: Släcker lysdioden.
   *
   *     - self: Pekare till lysdioden som ska släckas.
   ********************************************************************************/                   
   void (*off)(led_t* self);
   
   /********************************************************************************
   * toggle: Togglar utsignalen på lysdioder från tänd till släckt eller tvärtom.
   *
   *         - self: Pekare till lysdioden vars utsignal ska togglas.
   ********************************************************************************/                    
   void (*toggle)(led_t* self);  
   
   /********************************************************************************
   * blink: Blinkar lysdioden en gång med angiven blinkhastighet. För kontinuerlig
   *        blinkning måste denna funktion anropas i en fortgående loop.
   *
   *        - self: Pekare till lysdioden som ska blinkas.
   ********************************************************************************/                  
   void (*blink)(led_t* self, const uint16_t blink_speed_ms); 

} *led_vptr_t;


/********************************************************************************
* led_init: Initierar ny lysdiod på angiven pin.
*
*           - self: Pekare till lysdioden som ska initieras.
*           - pin : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8. 
*                   Alternativt kan motsvarande port-nummer på ATmega328P 
*                   anges, exempelvis B0 för pin 8 eller D2 för pin 2.
********************************************************************************/
void led_init(led_t* self, 
              const uint8_t pin);

/********************************************************************************
* led_clear: Nollställer lysdiod samt motsvarande pin.
*
*            - self: Pekare till lysdioden som ska nollställas.
********************************************************************************/
void led_clear(led_t* self);

/********************************************************************************
* led_new: Allokerar minne och initierar en ny lysdiod på angiven pin.
*          En pekare returneras till lysdioden efter initieringen.
*          Vid misslyckad minnesallokering returneras en nullpekare.
*
*          - pin : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8.
*                  Alternativt kan motsvarande port-nummer på ATmega328P
*                  anges, exempelvis B0 för pin 8 eller D2 för pin 2.
********************************************************************************/
led_t* led_new(const uint8_t pin);

/********************************************************************************
* led_delete: Frigör minne allokerat för angiven lysdiod och sätter motsvarande
*             pekare till null. För att kunna genomföra detta passeras adressen
*             till den pekare som pekar på lysdioden.
*
*             - self: Adressen till den pekare som pekar på lysdioden.
********************************************************************************/
void led_delete(led_t** self);

#endif /* LED_H_ */