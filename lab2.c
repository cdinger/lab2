/*
 * LAB 2
 * =====
 *
 * https://moodle2.umn.edu/mod/page/view.php?id=1359472
 *
 */
#include <pololu/orangutan.h>
#include <stdlib.h>
// #include <avr/io.h>

int COUNTS_PER_ROTATION = 64;

// // Motor Control Functions -- pwm is an 8-bit value
// //  (i.e. ranges from 0 to 255)
// void forward(unsigned char pwm)
// {
//   OCR2A = 0;
//   OCR2B = pwm;
// }
// 
// void reverse(unsigned char pwm)
// {
//   OCR2B = 0;
//   OCR2A = pwm;
// }
// 
// // Motor Initialization routine -- this function must be called
// //  before you use any of the above functions
// void motors_init()
// {
//   // configure for inverted PWM output on motor control pins:
//   //  set OCxx on compare match, clear on timer overflow
//   //  Timer0 and Timer2 count up from 0 to 255
//   TCCR0A = TCCR2A = 0xF3;
// 
//   // use the system clock/8 (=2.5 MHz) as the timer clock
//   TCCR0B = TCCR2B = 0x02;
// 
//   // initialize all PWMs to 0% duty cycle (braking)
//   OCR0A = OCR0B = OCR2A = OCR2B = 0;
// 
//   // set PWM pins as digital outputs (the PWM signals will not
//   // appear on the lines if they are digital inputs)
//   DDRD |= (1 << PORTD3) | (1 << PORTD5) | (1 << PORTD6);
//   DDRB |= (1 << PORTB3);
// }
// 
// #define F_CPU 20000000  // system clock is 20 MHz
// #include <util/delay.h>  // uses F_CPU to achieve us and ms delays
// 
// // delay for time_ms milliseconds by looping
// //  time_ms is a two-byte value that can range from 0 - 65535
// //  a value of 65535 (0xFF) produces an infinite delay
// void delay_ms(unsigned int time_ms)
// {
//   // _delay_ms() comes from <util/delay.h> and can only
//   //  delay for a max of around 13 ms when the system
//   //  clock is 20 MHz, so we define our own longer delay
//   //  routine based on _delay_ms()
//   unsigned int i;
//   for (i = 0; i < time_ms; i++)
//     _delay_ms(1);
// }

int G_ms_ticks = 0;
int G_release_pd = 0;
int G_current_speed = 0;

// This is the Interrupt Service Routine for Timer0
ISR(TIMER0_COMPA_vect) {
  G_ms_ticks++;
  G_release_pd = 1;
}

void debug(msg) {
  lcd_goto_xy(0,0);
  print_long(msg);
}

void initialize_pd_timer() {
  // set OC0A (PB3) to output mode
  DDRB |= (1 << DDB3);

  // Set waveform generation to CTC mode
  TCCR0A &= ~(0 << WGM00);
  TCCR0A |= (1 << WGM01);
  TCCR0B &= ~(0 << WGM02);

  // Toggle OC0A on Compare Match
  TCCR0A |= (1 << COM0A0);
  TCCR0A &= ~(0 << COM0A1);

  // set the prescaler = 256
  TCCR0B &= ~(0 << CS00);
  TCCR0B &= ~(0 << CS01);
  TCCR0B |= (1 << CS02);

  // Set output compare value = 78
  // 20M (processor clock) / 1000 / 256 = 78.12
  OCR0A = 78;

  // Timer/Counter0 Output Compare Match A Interrupt Enable
  TIMSK0 |= (1 << OCIE0A);

  // enable interrupts
  sei();
}

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
  PORTC |= (1 << PORTC6); // forward (clockwise)

  // Set TOP
  OCR2A = 0xff; // TODO: why does this need to be set at all??
  OCR2B = 0x00; // initialize at 0
}

// speed is between -100.0 and 100.0
//  100.0: full speed forwards/clockwise
// -100.0: full speed backwards/counterclockwise
void drive_motor(speed) {
  int pwm_top = abs((speed/100.0) * 255.0);

  if (speed >= 0.0) {
    PORTC |= (1 << PORTC6); // forward
  }
  else {
    PORTC &= ~(1 << PORTC6); // backwards
  }

  OCR2B = pwm_top;
}

void initialize_pd_controller() {
  initialize_pd_timer();

  // Initialize the encoders and specify the four input pins.
  // encoders_init(IO_C5, IO_C4, IO_C1, IO_C0);
  encoders_init(PINC5, PINC4, PINC1, PINC0);
}

void pd_control() {
  // T = Kp(Pr - Pm) - Kd*Vm
  //
  // T = Output motor signal (torque)
  // Pr = Desired motor position
  // Pm = Current motor position
  // Vm = Current motor velocity (computed based on finite differences)
  // Kp = Proportional gain
  // Kd = Derivative gain

  int Pm = encoders_get_counts_m2();
  int Pr = COUNTS_PER_ROTATION;
  float Kp = 10.0;
  float Kd = 0.1;
  int Vm = G_current_speed; // 1; // current motor velocity?
  int T = (Kp * (Pr - Pm)) - (Kd * Vm);

  lcd_goto_xy(0,1);
  print_long(T);

  set_motors(T, T); //motorSpeed, motorSpeed);
  G_current_speed = T;
}

int main() {
  clear();
  sei();
  initialize_motor();
  // initialize_pd_controller();

  while(1) {
    delay_ms(50);

    /*if (G_release_pd) {*/
      /*G_release_pd = 0;*/
      /*// execute pd controller*/
      /*pd_control();*/
      /*// lcd_goto_xy(0,0);*/
      /*// print_long(G_ms_ticks);*/
    /*}*/
  }
}

