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
                // something to do with mode 7? should try other fast PWM mode.
  OCR2B = 0x00; // initialize at 0
}

void drive_motor(int speed) {
  int pwm_top = 255;

  if (abs(speed) < 255) {
    pwm_top = abs(speed);
  }

  // stop the motor
  if (speed == 0) {
    // data direction for motor port = input
    DDRD &= ~(0 << PORTD6);
  }
  else {
    // data direction for motor port = output
    DDRD |= (1 << PORTD6);
    if (speed > 0) {
      PORTC = 0x00; // &= ~(0 << PORTC6); // forward
    }
    else {
      PORTC = 0xff; // |= (1 << PORTC6); // backwards
    }
  }

  OCR2B = pwm_top;
}
