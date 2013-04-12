#include <avr/io.h> //gives us names for registers
#include <stdlib.h>

void initialize_motor() {
  // clear everything
  TCCR2A = 0x00;
  TCCR2B = 0x00;

  // Fast PWM mode, TOP set by OCRA (mode 7)
  TCCR2B |= (1 << WGM22);
  TCCR2A |= (1 << WGM21);
  TCCR2A |= (1 << WGM20);

  // Clear OC2B on Compare Match, set OC2B at BOTTOM, (non-inverting mode).
  TCCR2A |= (1 << COM2B1);
  TCCR2A &= ~(0 << COM2B0);

  // No prescaler
  TCCR2B |= (1 << CS20);

  // data direction for motor port = output
  DDRD |= (1 << PORTD6);

  // data direction for motor direction pin = output
  DDRC |= (1 << PORTC6);
  PORTC &= ~(1 << PORTC6); // forward (clockwise)

  // Set TOP
  OCR2A = 0xff; // TODO: why does this need to be set at all??
  OCR2B = 0x00; // initialize at 0
}

// speed is between -100.0 and 100.0
//  100.0: full speed forwards/clockwise
// -100.0: full speed backwards/counterclockwise
void drive_motor(float speed) {
  // int pwm_top = abs((speed/100.0) * 255.0);
  int pwm_top = 255;
  if (abs(speed) < 255) {
    pwm_top = speed;
  }

  if (speed >= 0.0) {
    PORTC &= ~(1 << PORTC6); // forward
  }
  else {
    PORTC |= (1 << PORTC6); // backwards
  }

  OCR2B = pwm_top;
}
