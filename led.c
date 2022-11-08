/********************************************************************************
* led.h: Inneh�ller funktionsdefinitioner f�r implementering av lysdioder och
*        andra digitala utportar via strukten led.
********************************************************************************/
#include "led.h"

/* Statiska funktioner: */
static void led_on(led_t* self);
static void led_off(led_t* self);
static void led_toggle(led_t* self);
static void led_blink(led_t* self,
                      const uint16_t blink_speed_ms);
static led_vptr_t led_vptr_new(void);

/********************************************************************************
* led_init: Initierar ny lysdiod p� angiven pin.
*
*           - self: Pekare till lysdioden som ska initieras.
*           - pin : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
*                   Alternativt kan motsvarande port-nummer p� ATmega328P
*                   anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
void led_init(led_t* self,
              const uint8_t pin)
{
   if (pin >= 0 && pin <= 7)
   {
      self->io_port = IO_PORTD,
      self->pin = pin;
      set(DDRD, self->pin);
   }
   else if (pin >= 8 && pin <= 13)
   {
      self->io_port = IO_PORTB;
      self->pin = pin - 8;
      set(DDRB, self->pin);
   }
   else if (pin >= 14 && pin <= 19)
   {
      self->io_port = IO_PORTC;
      self->pin = pin - 14;
      set(DDRC, self->pin);
   }
   else
   {
      self->io_port = IO_PORT_NONE;
      self->pin = 0;
   }

   self->enabled = false;
   self->vptr = led_vptr_new();
   return;
}

/********************************************************************************
* led_clear: Nollst�ller lysdiod samt motsvarande pin.
*
*            - self: Pekare till lysdioden som ska nollst�llas.
********************************************************************************/
void led_clear(led_t* self)
{
   if (self->io_port == IO_PORTB)
   {
      clr(DDRB, self->pin);
      clr(PORTB, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      clr(DDRC, self->pin);
      clr(PORTC, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      clr(DDRD, self->pin);
      clr(PORTD, self->pin);
   }

   self->io_port = IO_PORT_NONE;
   self->pin = 0;
   self->enabled = false;
   return;
}

/********************************************************************************
* led_new: Allokerar minne och initierar en ny lysdiod p� angiven pin.
*          En pekare returneras till lysdioden efter initieringen.
*          Vid misslyckad minnesallokering returneras en nullpekare.
*
*          - pin : Lysdiodens pin-nummer p� Arduino Uno, exempelvis 8.
*                  Alternativt kan motsvarande port-nummer p� ATmega328P
*                  anges, exempelvis B0 f�r pin 8 eller D2 f�r pin 2.
********************************************************************************/
led_t* led_new(const uint8_t pin)
{
   led_t* self = (led_t*)malloc(sizeof(led_t));
   if (!self) return 0;
   led_init(self, pin);
   return self;
}

/********************************************************************************
* led_delete: Frig�r minne allokerat f�r angiven lysdiod och s�tter motsvarande
*             pekare till null. F�r att kunna genomf�ra detta passeras adressen
*             till den pekare som pekar p� lysdioden.
*
*             - self: Adressen till den pekare som pekar p� lysdioden.
********************************************************************************/
void led_delete(led_t** self)
{
   led_clear(*self);
   free(*self);
   *self = 0;
   return;
}

/********************************************************************************
* led_on: T�nder angiven lysdiod.
*
*         - self: Pekare till lysdioden som ska t�ndas.
********************************************************************************/
static void led_on(led_t* self)
{
   if (self->io_port == IO_PORTB)
   {
      set(PORTB, self->pin);
   }
   else if (self->io_port == IO_PORTC)
   {
      set(PORTC, self->pin);
   }
   else if (self->io_port == IO_PORTD)
   {
      set(PORTD, self->pin);
   }

   self->enabled = true;
   return;
}

/********************************************************************************
* led_off: Sl�cker angiven lysdiod.
*
*          - self: Pekare till lysdioden som ska sl�ckas.
********************************************************************************/
static void led_off(led_t* self)
{
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

   self->enabled = false;
   return;
}

/********************************************************************************
* led_toggle: Togglar utsignalen p� angiven lysdiod. Om lysdioden �r sl�ckt vid
*             anropet s� t�nds den. P� samma s�tt g�ller att om lysdioden �r
*             t�nd vid anropet s� sl�cks den.
*
*             - self: Pekare till lysdioden vars utsignal ska togglas.
********************************************************************************/
static void led_toggle(led_t* self)
{
   if (self->enabled)
   {
      led_off(self);
   }
   else
   {
      led_on(self);
   }

   return;
}

/********************************************************************************
* led_blink: Blinkar lysdiod en g�ng med angiven blinkhastighet.
*
*            - self          : Pekare till lysdioden som ska blinkas.
*            - blink_speed_ms: Blinkhastigheten m�tt i millisekunder.
********************************************************************************/
static void led_blink(led_t* self,
                      const uint16_t blink_speed_ms)
{
   led_toggle(self);
   delay_ms(blink_speed_ms);
   return;
}

/********************************************************************************
* led_vptr_new: Returnerar en pekare till ett vtable inneh�llande pekare till
*               associerade funktioner f�r strukten led. N�r programmet startar
*               s�tts respektive pekare till att peka p� motsvarande funktion.
********************************************************************************/
static led_vptr_t led_vptr_new(void)
{
   static struct led_vtable self = 
   {
      .on = led_on,
      .off = led_off, 
      .toggle = led_toggle,
      .blink = led_blink,
   };

   return &self;
}