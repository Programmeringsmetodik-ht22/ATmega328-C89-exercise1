/********************************************************************************
* button.h: Innehåller funktionsdefinitioner för enkel implementering av 
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
* button_init: Initierar ny tryckknapp på angiven pin.
*
*              - self: Pekare till tryckknappen som ska initieras.
*              - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 13.
*                      Alternativt kan motsvarande port-nummer på ATmega328P
*                      anges, exempelvis B5 för pin 13 eller D3 för pin 3.
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
* button_clear: Nollställer tryckknapp samt motsvarande pin.
*
*               - self: Pekare till tryckknappen som ska nollställas.
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
* button_new: Allokerar minne och initierar en ny tryckknapp på angiven pin.
*             En pekare returneras till tryckknappen efter initieringen.
*             Vid misslyckad minnesallokering returneras en nullpekare.
*
*             - pin : Tryckknappens pin-nummer på Arduino Uno, exempelvis 8.
*                     Alternativt kan motsvarande port-nummer på ATmega328P
*                     anges, exempelvis B0 för pin 8 eller D2 för pin 2.
********************************************************************************/
button_t* button_new(const uint8_t pin)
{
   button_t* self = (button_t*)malloc(sizeof(button_t));
   if (!self) return 0;
   button_init(self, pin);
   return self;
}

/********************************************************************************
* button_delete: Frigör minne allokerat för angiven tryckknapp och sätter
*                motsvarande pekare till null. För att kunna genomföra detta
*                passeras adressen till den pekare som pekar på tryckknappen.
*
*                - self: Adressen till den pekare som pekar på tryckknappen.
********************************************************************************/
void button_delete(button_t** self)
{
   button_clear(*self);
   free(*self);
   *self = 0;
   return;
}

/********************************************************************************
* button_is_pressed: Läser av tryckknappens pin och indikerar ifall denna är
*                    nedtryckt. I så fall returneras true, annars false.
*
*                    - self: Pekare till tryckknappen som ska läsas av.
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
* button_enable_interrupt: Aktiverar PCI-avbrott på angiven tryckknapp så att
*                          event på tryckknappens pin medför avbrott, både på
*                          stigande och fallande flank. Kontroll av flank måste
*                          skötas av användaren, förslagsvis via avläsning i
*                          motsvarande avbrottsrutin.
*
*                          Nedan visas sambandet mellan använd I/O-port samt
*                          avbrottsvektorn för motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras på.
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
* button_disable_interrupt: Inaktiverar PCI-avbrott på angiven tryckknapp.
*
*                           - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska inaktiveras på.
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
* button_toggle_interrupt: Togglar aktivering av PCI-avbrott på angiven
*                          tryckknapp. Om avbrott är aktiverat vid anrop
*                          så sker inaktivering. På samma sätt gäller att om
*                          avbrott är inaktiverat vid anrop så sker aktivering.
*
*                          - self: Pekare till tryckknappen som aktivering av
*                                  PCI-avbrott ska inaktiveras på.
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
* button_vptr_new: Returnerar en pekare till ett vtable innehållande pekare till
*                  associerade funktioner för strukten led. När programmet
*                  startar sätts respektive pekare till att peka på motsvarande
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