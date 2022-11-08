/********************************************************************************
* button.h: Inneh�ller funktionsdefinitioner f�r enkel implementering av 
*           tryckknappar samt andra digitala inportar via strukten button.
********************************************************************************/
#include "button.h"

/* Statiska funktioner: */
static bool button_is_pressed(const button_t* self);
static void button_enable_interrupt(button_t* self);
static void button_disable_interrupt(button_t* self);
static void button_toggle_interrupt(button_t* self);
static button_vptr_t button_vptr_new(void);

/********************************************************************************
* button_init: Initierar ny tryckknapp p� angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer p� ATmega328P
*                      anges, exempelvis B5 f�r pin 13 eller D3 f�r pin 3.
********************************************************************************/
void button_init(button_t* self,
                 const uint8_t pin)
{
   if (pin >= 0 && pin <= 7)
   {
      self->io_port = IO_PORTD;
      self->pin = pin;
      set(PORTD, self->pin);
   }
   else if (pin >= 8 && pin <= 13)
   {
      self->io_port = IO_PORTB;
      self->pin = pin - 8;
      set(PORTB, self->pin);
   }
   else if (pin >= 14 && pin <= 19)
   {
      self->io_port = IO_PORTC;
      self->pin = pin - 14;
      set(PORTC, self->pin);
   }
   else
   {
      self->io_port = IO_PORT_NONE;
      self->pin = 0;
   }

   self->interrupt_enabled = false;
   self->vptr = button_vptr_new();
   return;
}

/********************************************************************************
* button_clear: Nollst�ller tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollst�llas.
********************************************************************************/
void button_clear(button_t* self)
{
   button_disable_interrupt(self);

   if (self->io_port == IO_PORTB)
   {
      clr(PORTB, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      clr(PORTC, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      clr(PORTD, self->pin);
   }

   self->io_port = IO_PORT_NONE;
   self->pin = 0;
   return;
}

/********************************************************************************
* button_new: Allokerar minne och initierar en ny tryckknapp p� angiven pin.
*             En pekare returneras till tryckknappen efter initieringen.
*             Vid misslyckad minnesallokering returneras en nullpekare.
*
*             - pin : Tryckknappens pin-nummer p� Arduino Uno, exempelvis 8.
*                     Alternativt kan motsvarande port-nummer p� ATmega328P
*                     anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
button_t* button_new(const uint8_t pin)
{
   button_t* self = (button_t*)malloc(sizeof(button_t));
   if (!self) return 0;
   button_init(self, pin);
   return self;
}

/********************************************************************************
* button_delete: Frig�r minne allokerat f�r angiven tryckknapp och s�tter
*                motsvarande pekare till null. F�r att kunna genomf�ra detta
*                passeras adressen till den pekare som pekar p� tryckknappen.
*
*                - self: Adressen till den pekare som pekar p� tryckknappen.
********************************************************************************/
void button_delete(button_t** self)
{
   button_clear(*self);
   free(*self);
   *self = 0;
   return;
}

/********************************************************************************
* button_is_pressed: L�ser av tryckknappens pin och indikerar ifall denna �r
*                    nedtryckt. I s� fall returneras true, annars false.
*
*                    - self: Pekare till tryckknappen som ska l�sas av.
********************************************************************************/
static bool button_is_pressed(const button_t* self)
{
   if (self->io_port == IO_PORTB)
   {
      return read(PINB, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      return read(PINC, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      return read(PIND, self->pin);
   }
   else
   {
      return false;
   }
}

/********************************************************************************
* button_enable_interrupt: Aktiverar PCI-avbrott p� angiven tryckknapp s� att
*                          event p� tryckknappens pin medf�r avbrott, b�de p�
*                          stigande och fallande flank. Kontroll av flank m�ste
*                          sk�tas av anv�ndaren, f�rslagsvis via avl�sning i
*                          motsvarande avbrottsrutin.
*
*                          Nedan visas sambandet mellan anv�nd I/O-port samt
*                          avbrottsvektorn f�r motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras p�.
********************************************************************************/
static void button_enable_interrupt(button_t* self)
{
   sei();

   if (self->io_port == IO_PORTB)
   {
      set(PCICR, PCIE0);
      set(PCMSK0, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      set(PCICR, PCIE1);
      set(PCMSK1, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      set(PCICR, PCIE2);
      set(PCMSK2, self->pin);
   }

   self->interrupt_enabled = true;
   return;
}

/********************************************************************************
* button_disable_interrupt: Inaktiverar PCI-avbrott p� angiven tryckknapp.
*
*                           - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska inaktiveras p�.
********************************************************************************/
static void button_disable_interrupt(button_t* self)
{
   if (self->io_port == IO_PORTB)
   {
      clr(PCMSK0, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      clr(PCMSK1, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      clr(PCMSK2, self->pin);
   }

   self->interrupt_enabled = false;
   return;
}

/********************************************************************************
* button_toggle_interrupt: Togglar aktivering av PCI-avbrott p� angiven
*                          tryckknapp. Om avbrott �r aktiverat vid anrop
*                          s� sker inaktivering. P� samma s�tt g�ller att om
*                          avbrott �r inaktiverat vid anrop s� sker aktivering.
*
*                          - self: Pekare till tryckknappen som aktivering av
*                                  PCI-avbrott ska inaktiveras p�.
********************************************************************************/
static void button_toggle_interrupt(button_t* self)
{
   if (self->interrupt_enabled)
   {
      button_disable_interrupt(self);
   }
   else
   {
      button_enable_interrupt(self);
   }

   return;
}

/********************************************************************************
* button_vptr_new: Returnerar en pekare till ett vtable inneh�llande pekare till
*                  associerade funktioner f�r strukten led. N�r programmet
*                  startar s�tts respektive pekare till att peka p� motsvarande
*                  funktion.
********************************************************************************/
static button_vptr_t button_vptr_new(void)
{
   static struct button_vtable self = 
   {
      .is_pressed = button_is_pressed,
      .enable_interrupt = button_enable_interrupt,
      .disable_interrupt = button_disable_interrupt,
      .toggle_interrupt = button_toggle_interrupt
   };

   return &self;
}