/*
 * LAB 2
 * =====
 *
 * https://moodle2.umn.edu/mod/page/view.php?id=1359472
 *
 */
#include <pololu/orangutan.h>
#include <stdio.h>
#include "lab2.h"
#include "motors.h"
#include "menu.h"

int G_ms_ticks = 0;
int G_release_pd = 0;
int G_current_speed = 0;
int G_max_speed = 0;
int G_previous_counts = 0;
int G_previous_T = 0;
int G_pd_frequency = 1000;

// This is the Interrupt Service Routine for Timer0
ISR(TIMER0_COMPA_vect) {
  G_ms_ticks++;
  if ((int)(G_ms_ticks % (1000 / G_pd_frequency)) == 0) {
    G_release_pd = 1;
  }
}

void debug(msg) {
  lcd_goto_xy(0,0);
  print_long(msg);
}

void toggle_logging() {
  G_logging_enabled = !G_logging_enabled;
  if (G_logging_enabled) {
    print_usb("\n\rLogging enabled.", 18);
  }
  else {
    print_usb("\n\rLogging disabled.", 19);
  }
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
  // 20M / 50 / 256
  OCR0A = 78;

  // Timer/Counter0 Output Compare Match A Interrupt Enable
  TIMSK0 |= (1 << OCIE0A);
}


void initialize_pd_controller() {
  initialize_pd_timer();

  // Initialize the encoders and specify the four input pins.
  encoders_init(IO_C5, IO_C4, IO_C1, IO_C0);
}

int degrees_in_wheel_ticks(int degrees) {
  return (degrees / (360 / COUNTS_PER_ROTATION));
}

int wheel_ticks_in_degrees(int wheel_ticks) {
  return (wheel_ticks * (360 / COUNTS_PER_ROTATION));
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

  char printBuffer[64];
  int length;
  int current_counts = encoders_get_counts_m2();
  G_Pm = current_counts;
  G_Pr = degrees_in_wheel_ticks(relative_degrees);
  G_Vm = G_current_speed; // TODO: current motor velocity?
  // G_Vm = ((G_ms_ticks * current_counts) - (G_ms_ticks * G_previous_counts));
  G_T = (G_Kp * (G_Pr - G_Pm)) - (int)(((float)G_Kd/10.0f) * (float)G_Vm);

  drive_motor(G_T);

  // every 100ms
  if ((G_ms_ticks % 100) == 0) {
    if (G_logging_enabled) {
      print_current_values();
    }
  }

  if (G_T > G_max_speed) {
    G_max_speed = G_T;
  }

  G_current_speed = G_T;
  G_previous_counts = current_counts;

}

void print_current_values() {
  int length;
  char tempBuffer[64];
  length = sprintf(tempBuffer, "\n\rF=%d S=%d Kd=%d/10 Kp=%d Vm=%d Pr=%d Pm=%d T=%d", G_pd_frequency, G_degree_step_size, G_Kd, G_Kp, G_Vm, G_Pr, G_Pm, G_T);
  print_usb(tempBuffer, length);
}

int min(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

void interpolate_trajectory() {
  // G_Pm = current counts
  int target;
  int pm_degrees = wheel_ticks_in_degrees(G_Pm);

  if (G_absolute_degrees >= pm_degrees) {
    target = pm_degrees + min(G_degree_step_size, G_absolute_degrees - pm_degrees);
  }
  else {
    target = pm_degrees - min(G_degree_step_size, pm_degrees - G_absolute_degrees);
  }
  pd_control(target);
}

int main() {
  sei();
  clear();
  lcd_init_printf();
  initialize_motor();
  initialize_pd_controller();
  init_menu();

  G_logging_enabled = 0;
  G_absolute_degrees = 0;
  G_degree_step_size = 45;
  G_Kp = 20;
  G_Kd = 1;

  while(1) {
    if (G_release_pd) {
      G_release_pd = 0;
      // execute pd controller
      interpolate_trajectory();
    }
    serial_check();
    check_for_new_bytes_received();
  }
}

