/********************************************************************************
* led.h: Innehåller funktionsdefinitioner för implementering av lysdioder och
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
* led_init: Initierar ny lysdiod på angiven pin.
*
*           - self: Pekare till lysdioden som ska initieras.
*           - pin : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8.
*                   Alternativt kan motsvarande port-nummer på ATmega328P
*                   anges, exempelvis B0 för pin 8 eller D2 för pin 2.
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
* led_clear: Nollställer lysdiod samt motsvarande pin.
*
*            - self: Pekare till lysdioden som ska nollställas.
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
* led_new: Allokerar minne och initierar en ny lysdiod på angiven pin.
*          En pekare returneras till lysdioden efter initieringen.
*          Vid misslyckad minnesallokering returneras en nullpekare.
*
*          - pin : Lysdiodens pin-nummer på Arduino Uno, exempelvis 8.
*                  Alternativt kan motsvarande port-nummer på ATmega328P
*                  anges, exempelvis B0 för pin 8 eller D2 för pin 2.
********************************************************************************/
led_t* led_new(const uint8_t pin)
{
   led_t* self = (led_t*)malloc(sizeof(led_t));
   if (!self) return 0;
   led_init(self, pin);
   return self;
}

/********************************************************************************
* led_delete: Frigör minne allokerat för angiven lysdiod och sätter motsvarande
*             pekare till null. För att kunna genomföra detta passeras adressen
*             till den pekare som pekar på lysdioden.
*
*             - self: Adressen till den pekare som pekar på lysdioden.
********************************************************************************/
void led_delete(led_t** self)
{
   led_clear(*self);
   free(*self);
   *self = 0;
   return;
}

/********************************************************************************
* led_on: Tänder angiven lysdiod.
*
*         - self: Pekare till lysdioden som ska tändas.
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
* led_off: Släcker angiven lysdiod.
*
*          - self: Pekare till lysdioden som ska släckas.
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
* led_toggle: Togglar utsignalen på angiven lysdiod. Om lysdioden är släckt vid
*             anropet så tänds den. På samma sätt gäller att om lysdioden är
*             tänd vid anropet så släcks den.
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
* led_blink: Blinkar lysdiod en gång med angiven blinkhastighet.
*
*            - self          : Pekare till lysdioden som ska blinkas.
*            - blink_speed_ms: Blinkhastigheten mätt i millisekunder.
********************************************************************************/
static void led_blink(led_t* self,
                      const uint16_t blink_speed_ms)
{
   led_toggle(self);
   delay_ms(blink_speed_ms);
   return;
}

/********************************************************************************
* led_vptr_new: Returnerar en pekare till ett vtable innehållande pekare till
*               associerade funktioner för strukten led. När programmet startar
*               sätts respektive pekare till att peka på motsvarande funktion.
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