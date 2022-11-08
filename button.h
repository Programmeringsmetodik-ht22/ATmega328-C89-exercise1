/********************************************************************************
* button.h: Inneh�ller funktionalitet f�r enkel implementering av tryckknappar
*           via strukten button samt associerade funktioner, som n�s via en 
*           vtable-pekare. Denna strukt fungerar ocks� utm�rkt f�r andra 
*           digitala inportar d�r insignalen ska kunna l�sas av samt avbrott 
*           ska kunna genereras vid ett godtyckligt event.
********************************************************************************/
#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

struct button_vtable; /* F�rdeklarerar inf�r deklaration av strukten button. */

/********************************************************************************
* button: Strukt f�r implementering av tryckknappar och andra digitala inportar.
*         PCI-avbrott kan aktiveras p� aktuell pin. D�rmed f�r eventdetektering 
*         implementeras av anv�ndaren, d� PCI-avbrott inte m�jligg�r kontroll
*         av vilken flank som avbrott ska ske p�.
********************************************************************************/
typedef struct button
{
   uint8_t pin;                /* Tryckknappens pin-nummer p� aktuell I/O-port. */
   enum io_port io_port;       /* I/O-port som lysdioden �r ansluten till. */
   bool interrupt_enabled;     /* Indikerar ifall PCI-avbrott �r aktiverat. */
   struct button_vtable* vptr; /* Pekare till vtable inneh�llande associerade funktioner. */
} button_t, *button_ptr_t;

/********************************************************************************
* button_vtable: Strukt f�r vtable inneh�llande funktionspekare till associerade
*                funktioner f�r strukten button.
********************************************************************************/
typedef struct button_vtable
{
   /********************************************************************************
   * is_pressed: L�ser av tryckknappens pin och indikerar ifall denna �r nedtryckt.
   *             I s� fall returneras true, annars false.
   *
   *             - self: Pekare till tryckknappen som ska l�sas av.
   ********************************************************************************/
   bool (*is_pressed)(const button_t* self);

   /********************************************************************************
   * enable_interrupt: Aktiverar PCI-avbrott p� angiven tryckknapp s� att event p�
   *                   tryckknappens pin medf�r avbrott, b�de p� stigande och 
   *                   fallande flank. Kontroll av flank m�ste sk�tas av anv�ndaren, 
   *                   f�rslagsvis via avl�sning i motsvarande avbrottsrutin.
   *
   *                   Nedan visas sambandet mellan anv�nd I/O-port samt
   *                   avbrottsvektorn f�r motsvarande avbrottsrutin:
   *
   *                   I/O-port     pin (Arduino Uno)     Avbrottsvektor
   *                      B              8 - 13             PCINT0_vect
   *                      C             A0 - A5             PCINT1_vect
   *                      D              0 - 7              PCINT2_vect
   *
   *                      - self: Pekare till tryckknappen som PCI-avbrott ska
   *                              aktiveras p�.
   ********************************************************************************/
   void (*enable_interrupt)(button_t* self);

   /********************************************************************************
   * disable_interrupt: Inaktiverar PCI-avbrott p� angiven tryckknapp.
   *
   *                    - self: Pekare till tryckknappen som PCI-avbrott ska
   *                            inaktiveras p�.
   ********************************************************************************/
   void (*disable_interrupt)(button_t* self);

   /********************************************************************************
   * toggle_interrupt: Togglar aktivering av PCI-avbrott p� angiven tryckknapp. 
   *                   Om avbrott �r aktiverat vid anrop s� sker inaktivering. 
   *                   P� samma s�tt g�ller att om avbrott �r inaktiverat vid anrop 
   *                   s� sker aktivering.
   *
   *                   - self: Pekare till tryckknappen som aktivering av 
   *                           PCI-avbrott ska inaktiveras p�.
   ********************************************************************************/
   void (*toggle_interrupt)(button_t* self);

} button_vtable_t, *button_vptr_t;

/********************************************************************************
* button_init: Initierar ny tryckknapp p� angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer p� ATmega328P
*                      anges, exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
********************************************************************************/
void button_init(button_t* self,
                 const uint8_t pin);

/********************************************************************************
* button_clear: Nollst�ller tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollst�llas.
********************************************************************************/
void button_clear(button_t* self);

/********************************************************************************
* button_new: Allokerar minne och initierar en ny tryckknapp p� angiven pin.
*             En pekare returneras till tryckknappen efter initieringen.
*             Vid misslyckad minnesallokering returneras en nullpekare.
*
*             - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 8.
*                     Alternativt kan motsvarande port-nummer p� ATmega328P
*                     anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
button_t* button_new(const uint8_t pin);

/********************************************************************************
* button_delete: Frig�r minne allokerat f�r angiven tryckknapp och s�tter 
*                motsvarande pekare till null. F�r att kunna genomf�ra detta 
*                passeras adressen till den pekare som pekar p� tryckknappen.
*
*                - self: Adressen till den pekare som pekar p� tryckknappen.
********************************************************************************/
void button_delete(button_t** self);

#endif /* BUTTON_H_ */