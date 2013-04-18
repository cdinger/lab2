#include <pololu/orangutan.h>
#include <stdio.h>

#include "menu.h"
#include "lab2.h"

// #define ECHO2LCD

char receive_buffer[32];
unsigned char receive_buffer_position;
char send_buffer[32];
int end_of_last_command = -1;

void print_usb(char *buffer, int n) {
  serial_send(USB_COMM, buffer, n);
  wait_for_sending_to_finish();
}

void init_menu() {
  char printBuffer[32];

  // Set the baud rate to 9600 bits per second.  Each byte takes ten bit
  // times, so you can get at most 960 bytes per second at this speed.
  serial_set_baud_rate(USB_COMM, 9600);

  // Start receiving bytes in the ring buffer.
  serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));

  print_usb("\n\rUSB Serial Initialized\r\n", 24);
  print_usb(MENU, MENU_LENGTH);
}

// Parses a menu command (series of keystrokes) that has been received on
// USB_COMM and processes it accordingly. The menu command is buffered in
// check_for_new_bytes_received (which calls this function).
//
void process_received_string(const char* buffer) {
  int length;
  char tempBuffer[64];
  char op_char;
  int value;
  int parsed;
  int old_Kp;
  int old_Kd;

  print_usb(buffer, strlen(buffer));
  parsed = sscanf(buffer, "%c %d", &op_char, &value);
#ifdef ECHO2LCD
  lcd_goto_xy(0,0);
  printf("Got %c %d\n", op_char, value);
#endif

  // Check valid command and implement
  switch (op_char) {
    case 'x':
      print_long(G_Kp);
      break;
    case 'L':
      // Start/Stop Logging (print) the values of Pr, Pm, and T.
      toggle_logging();
    break;
    case 'V':
      // View the current values Kd, Kp, Vm, Pr, Pm, and T
      length = sprintf(tempBuffer, "\n\rKd=%d/10 Kp=%d Vm=%d Pr=%d Pm=%d T=%d", G_Kd, G_Kp, G_Vm, G_Pr, G_Pm, G_T);
      print_usb(tempBuffer, length);
      break;
    case 'R':
      // Set the reference position to degrees
      G_relative_degrees = value;
      break;
    case 'P':
      // Increase Kp by an amount of your choice*
      old_Kp = G_Kp;
      G_Kp += 1;
      length = sprintf(tempBuffer, "\n\rKp was %d, is now %d", old_Kp, G_Kp);
      print_usb(tempBuffer, length);
      break;
    case 'p':
      // Decrease Kp by an amount of your choice
      old_Kp = G_Kp;
      G_Kp -= 1;
      length = sprintf(tempBuffer, "\n\rKp was %d, is now %d", old_Kp, G_Kp);
      print_usb(tempBuffer, length);
      break;
    case 'D':
      // Increase Kd by an amount of your choice
      old_Kd = G_Kd;
      G_Kd += 1;
      length = sprintf(tempBuffer, "\n\rKd was %d, is now %d", old_Kd, G_Kd);
      print_usb(tempBuffer, length);
      break;
    case 'd':
      // Decrease Kd by an amount of your choice
      old_Kd = G_Kd;
      G_Kd -= 1;
      length = sprintf(tempBuffer, "\n\rKd was %d, is now %d", old_Kd, G_Kd);
      print_usb(tempBuffer, length);
      break;
    default:
      length = sprintf(tempBuffer, "\n\rMalformed command: %s", buffer);
      print_usb(tempBuffer, length);
  }

  print_usb( MENU, MENU_LENGTH);
}

// Loops through the receive_buffer, accumulating new bytes (keystrokes) in
// another buffer for processing.
//
void check_for_new_bytes_received() {

  while(serial_get_received_bytes(USB_COMM) != receive_buffer_position) {
    if (receive_buffer[receive_buffer_position] == 0xD) {
      char menuBuffer[32];

      if (end_of_last_command < receive_buffer_position) {
        int i = end_of_last_command + 1;
        int j = 0;
        for (i + 1; i < receive_buffer_position; i++) {
          menuBuffer[j] = receive_buffer[i];
          j++;
        }
        menuBuffer[j] = '\0';
      }

      end_of_last_command = receive_buffer_position;

#ifdef ECHO2LCD
      clear();
      lcd_goto_xy(0,1);
      print(menuBuffer);
#endif

      process_received_string(menuBuffer);
    }

    // Increment receive_buffer_position, but wrap around when it gets to the
    // end of the buffer
    if ( receive_buffer_position == sizeof(receive_buffer) - 1 ) {
      receive_buffer_position = 0;
    }
    else {
      receive_buffer_position++;
    }
  }
}

// Waits for the bytes in the send buffer to finish transmitting on USB_COMM.
// We must call this before modifying send_buffer or trying to send more
// bytes, because otherwise we could corrupt an existing transmission.
//
void wait_for_sending_to_finish() {
  while(!serial_send_buffer_empty(USB_COMM)) {
    serial_check(); // USB_COMM port is always in SERIAL_CHECK mode
  }
}


