/********************************************************************************
* button.h: Innehåller funktionalitet för enkel implementering av tryckknappar
*           via strukten button samt associerade funktioner, som nås via en 
*           vtable-pekare. Denna strukt fungerar också utmärkt för andra 
*           digitala inportar där insignalen ska kunna läsas av samt avbrott 
*           ska kunna genereras vid ett godtyckligt event.
********************************************************************************/
#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

struct button_vtable; /* Fördeklarerar inför deklaration av strukten button. */

/********************************************************************************
* button: Strukt för implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras på aktuell pin. Därmed får eventdetektering 
*         implementeras av användaren, då PCI-avbrott inte möjliggör kontroll
*         av vilken flank som avbrott ska ske på.
********************************************************************************/
typedef struct button
{
   uint8_t pin;                /* Tryckknappens pin-nummer på aktuell I/O-port. */
   enum io_port io_port;       /* I/O-port som lysdioden är ansluten till. */
   bool interrupt_enabled;     /* Indikerar ifall PCI-avbrott är aktiverat. */
   struct button_vtable* vptr; /* Pekare till vtable innehållande associerade funktioner. */
} button_t, *button_ptr_t;

/********************************************************************************
* button_vtable: Strukt för vtable innehållande funktionspekare till associerade
*                funktioner för strukten button.
********************************************************************************/
typedef struct button_vtable
{
   /********************************************************************************
   * is_pressed: Läser av tryckknappens pin och indikerar ifall denna är nedtryckt.
   *             I så fall returneras true, annars false.
   *
   *             - self: Pekare till tryckknappen som ska läsas av.
   ********************************************************************************/
   bool (*is_pressed)(const button_t* self);

   /********************************************************************************
   * enable_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att event på
   *                   tryckknappens pin medför avbrott, både på stigande och 
   *                   fallande flank. Kontroll av flank måste skötas av användaren, 
   *                   förslagsvis via avläsning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan använd I/O-port samt
   *                   avbrottsvektorn för motsvarande avbrottsrutin:
   *
   *                   I/O-port     pin (Arduino Uno)     Avbrottsvektor
   *                      B              8 - 13             PCINT0_vect
   *                      C             A0 - A5             PCINT1_vect
   *                      D              0 - 7              PCINT2_vect
   *
   *                      - self: Pekare till tryckknappen som PCI-avbrott ska
   *                              aktiveras på.
   ********************************************************************************/
   void (*enable_interrupt)(button_t* self);

   /********************************************************************************
   * disable_interrupt: Inaktiverar PCI-avbrott på angiven tryckknapp.
   *
   *                    - self: Pekare till tryckknappen som PCI-avbrott ska
   *                            inaktiveras på.
   ********************************************************************************/
   void (*disable_interrupt)(button_t* self);

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av PCI-avbrott på angiven tryckknapp. 
   *                   Om avbrott är aktiverat vid anrop så sker inaktivering. 
   *                   På samma sätt gäller att om avbrott är inaktiverat vid anrop 
   *                   så sker aktivering.
   *
   *                   - self: Pekare till tryckknappen som aktivering av 
   *                           PCI-avbrott ska inaktiveras på.
   ********************************************************************************/
   void (*toggle_interrupt)(button_t* self);

} button_vtable_t, *button_vptr_t;

/********************************************************************************
* button_init: Initierar ny tryckknapp på angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer på ATmega328P
*                      anges, exempelvis B5 för pin 13 eller D3 för pin 3.
********************************************************************************/
void button_init(button_t* self,
                 const uint8_t pin);

/********************************************************************************
* button_clear: Nollställer tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollställas.
********************************************************************************/
void button_clear(button_t* self);

/********************************************************************************
* button_new: Allokerar minne och initierar en ny tryckknapp på angiven pin.
*             En pekare returneras till tryckknappen efter initieringen.
*             Vid misslyckad minnesallokering returneras en nullpekare.
*
*             - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 8.
*                     Alternativt kan motsvarande port-nummer på ATmega328P
*                     anges, exempelvis B0 för pin 8 eller D2 för pin 2.
********************************************************************************/
button_t* button_new(const uint8_t pin);

/********************************************************************************
* button_delete: Frigör minne allokerat för angiven tryckknapp och sätter 
*                motsvarande pekare till null. För att kunna genomföra detta 
*                passeras adressen till den pekare som pekar på tryckknappen.
*
*                - self: Adressen till den pekare som pekar på tryckknappen.
********************************************************************************/
void button_delete(button_t** self);

#endif /* BUTTON_H_ */