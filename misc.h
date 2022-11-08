/********************************************************************************
* misc.h: Innehåller diverse bibliotek, definitioner samt enumerationer för
*         implementering av mikrodatorsystemet.
********************************************************************************/
#ifndef MISC_H_
#define MISC_H_

/* Inkluderingsdirektiv: */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

/* Makrodefinitioner för port-nummer på ATmega328P samt motsvarande pin-nummer på Arduino Uno: */
#define D0 0 /* PORTD0 / pin 0. */
#define D1 1 /* PORTD1 / pin 1. */
#define D2 2 /* PORTD2 / pin 2. */
#define D3 3 /* PORTD3 / pin 3. */
#define D4 4 /* PORTD4 / pin 4. */
#define D5 5 /* PORTD5 / pin 5. */
#define D6 6 /* PORTD6 / pin 6. */
#define D7 7 /* PORTD7 / pin 7. */

#define B0 8  /* PORTB0 / pin 8. */
#define B1 9  /* PORTB1 / pin 9. */
#define B2 10 /* PORTB2 / pin 10. */
#define B3 11 /* PORTB3 / pin 11. */
#define B4 12 /* PORTB4 / pin 12. */
#define B5 13 /* PORTB5 / pin 13. */

#define C0 14 /* PORTC0 / pin A0. */
#define C1 15 /* PORTC1 / pin A1. */
#define C2 16 /* PORTC2 / pin A2. */
#define C3 17 /* PORTC3 / pin A3. */
#define C4 18 /* PORTC4 / pin A4. */
#define C5 19 /* PORTC5 / pin A5. */

#define A0 14 /* PORTC0 / pin A0. */
#define A1 15 /* PORTC1 / pin A1. */
#define A2 16 /* PORTC2 / pin A2. */
#define A3 17 /* PORTC3 / pin A3. */
#define A4 18 /* PORTC4 / pin A4. */
#define A5 19 /* PORTC5 / pin A5. */

/********************************************************************************
* bool: Enumeration för booleska värden 0 och 1.
********************************************************************************/
typedef enum { false, true } bool;

/********************************************************************************
* io_port: Enumeration för val av I/O-port mellan I/O-portar B, C och D.
********************************************************************************/
enum io_port
{
   IO_PORTB,    /* I/O-port B (pin 8 - 13 på Arduino Uno). */
   IO_PORTC,    /* I/O-port C (pin A0 - A5 på Arduino Uno). */
   IO_PORTD,    /* I/O-port D (pin 0 - 7 på Arduino Uno). */
   IO_PORT_NONE /* Icke-specificerad I/O-port. */
};

/********************************************************************************
* set: Ettställer bit i angivet register utan att påverka övriga bitar.
*
*      - reg: Registret vars innehåll ska modifieras.
*      - bit: Biten i registret som ska ettställas.
********************************************************************************/
#define set(reg, bit) reg |= (1 << bit)

/********************************************************************************
* clr: Nollställer bit i angivet register utan att påverka övriga bitar.
*
*      - reg: Registret vars innehåll ska modifieras.
*      - bit: Biten i registret som ska nollställas.
********************************************************************************/
#define clr(reg, bit) reg &= ~(1 << bit)

/********************************************************************************
* read: Läser av bit i angivet register, returnerar värdet som logiska nolla
*       eller etta via booleska värden true och false.
*
*      - reg: Registret där biten som ska läsas av ligger i.
*      - bit: Biten i registret som ska läsas av.
********************************************************************************/
#define read(reg, bit) (bool)(reg & (1 << (bit)))

/********************************************************************************
* delay_ms: Genererar fördröjning mätt i millisekunder.
*
*           - delay_time_ms: Angiven fördröjningstid i millisekunder.
********************************************************************************/
#define delay_ms(delay_time_ms) ({ \
   volatile uint16_t i, j; \
   for (i = 0; i < delay_time_ms; ++i) { \
      for (j = 0; j < 1000; ++j) ; \
   } \
})

#endif /* MISC_H_ */