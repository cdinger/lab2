/*
 * LAB 2
 * =====
 *
 * https://moodle2.umn.edu/mod/page/view.php?id=1359472
 *
 */
#include <pololu/orangutan.h>
#include <stdio.h>
#include "motors.h"
#include "menu.h"

int G_ms_ticks = 0;
int G_release_pd = 0;
int G_current_speed = 0;
int G_max_speed = 0;
int G_previous_counts = 0;
int G_previous_T = 0;

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


void initialize_pd_controller() {
  initialize_pd_timer();

  // Initialize the encoders and specify the four input pins.
  encoders_init(IO_C5, IO_C4, IO_C1, IO_C0);
}

int degrees_in_wheel_ticks(int degrees) {
  float degrees_per_tick = (360.0 / COUNTS_PER_ROTATION);
  return (degrees/degrees_per_tick);
}

void pd_control(int relative_degrees) {
  // T = Kp(Pr - Pm) - Kd*Vm
  //
  // T = Output motor signal (torque)
  // Pr = Desired motor position
  // Pm = Current motor position
  // Vm = Current motor velocity (computed based on finite differences)
  // Kp = Proportional gain
  // Kd = Derivative gain

  int current_counts = encoders_get_counts_m2();
  int Pm = current_counts;
  int Pr = degrees_in_wheel_ticks(relative_degrees);
  float Kp = 4.0;
  float Kd = 0.1;
  // int Vm = G_current_speed; // TODO: current motor velocity?
  int Vm = ((G_ms_ticks * current_counts) - (G_ms_ticks * G_previous_counts));
  int T = (Kp * (Pr - Pm)) - (Kd * Vm);

  drive_motor(T);

  if (T > G_max_speed) {
    G_max_speed = T;
  }

  // every 100ms
  if ((G_ms_ticks % 100) == 0) {
    // print_usb("blah.\n\r");
  }

  G_previous_counts = current_counts;
}

int interpolate_trajetory() {
  return 0;
}

int main() {
  sei();
  clear();
  lcd_init_printf();
  initialize_motor();
  initialize_pd_controller();
  // initialize_serial();
  init_menu();

  while(1) {
    /*if (G_release_pd) {*/
      /*G_release_pd = 0;*/
      /*// execute pd controller*/
      /*pd_control(720);*/
      /*// lcd_goto_xy(0,0);*/
      /*// print_long(G_ms_ticks);*/
    /*}*/

    serial_check();
    check_for_new_bytes_received();
  }
}

