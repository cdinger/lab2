#include <pololu/orangutan.h>

int G_logging_enabled = 0;

// receive_buffer: A ring buffer that we will use to receive bytes on USB_COMM.
// The OrangutanSerial library will put received bytes in to
// the buffer starting at the beginning (receiveBuffer[0]).
// After the buffer has been filled, the library will automatically
// start over at the beginning.
char receive_buffer[32];

// receive_buffer_position: This variable will keep track of which bytes in the receive buffer
// we have already processed.  It is the offset (0-31) of the next byte
// in the buffer to process.
unsigned char receive_buffer_position = 0;

// send_buffer: A buffer for sending bytes on USB_COMM.
char send_buffer[32];

// wait_for_sending_to_finish:  Waits for the bytes in the send buffer to
// finish transmitting on USB_COMM.  We must call this before modifying
// send_buffer or trying to send more bytes, because otherwise we could
// corrupt an existing transmission.
void wait_for_sending_to_finish() {
  while(!serial_send_buffer_empty(USB_COMM))
    serial_check();  // USB_COMM port is always in SERIAL_CHECK mode
}

// A generic function for whenever you want to print to your serial comm window.
// Provide a string and the length of that string. My serial comm likes "\r\n" at 
// the end of each string (be sure to include in length) for proper linefeed.
void print_usb(const char *buffer) {
  serial_send(USB_COMM, (char*)buffer, strlen(buffer));
  wait_for_sending_to_finish();
}


/* - L/l: Start/Stop Logging (print) the values of Pr, Pm, and T.
 * - V/v: View the current values Kd, Kp, Vm, Pr, Pm, and T
 * - R/r : Set the reference position to degrees
 * - P: Increase Kp by an amount of your choice*
 * - p: Decrease Kp by an amount of your choice
 * - D: Increase Kd by an amount of your choice
 * - d: Decrease Kd by an amount of your choice
 */
void show_menu() {
  lcd_goto_xy(0, 0);
  print_usb("Enter command (L,V,R,P|p,D|d):");
}

void initialize_serial() {
  // Set the baud rate to 9600 bits per second.  Each byte takes ten bit
  // times, so you can get at most 960 bytes per second at this speed.
  serial_set_baud_rate(USB_COMM, 9600);

  // Start receiving bytes in the ring buffer.
  serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));
  show_menu();
}

void toggle_logging() {
  G_logging_enabled = !G_logging_enabled;
  if (G_logging_enabled) {
    print_usb("Logging enabled.");
  }
  else {
    print_usb("Logging disabled.");
  }
  print_usb("\n\r");
  show_menu();
}

void show_current_values() {
  // TODO
}

void set_reference_position() {
  // TODO
}

void process_received_bytes(const char* command) {
  clear(); // clear LCD
  lcd_goto_xy(0, 1);
  wait_for_sending_to_finish();
  print_usb(command);
  print_usb("\n\r");
  char op_char;
  int value;
  int parsed = 0;
  parsed = sscanf(command, "%c %d", &op_char, &value);

  switch (op_char) {
    case 'l':
      break;
    default:
      print_usb("Unknown command: ");
      print_usb(op_char);
      print_usb("\n\r");
      show_menu();
  }
    /*
    case "L":
    case "l":
      toggle_logging();
      break;
    case "V":
    case "v":
      show_current_values();
      break;
    case "R":
    case "r":
      set_reference_position();
      break;
    case "P":
      break;
    case "p":
      break;
    case "D":
      break;
    case "d":
      break;
    default:
  }
  // print_usb("printing buffer");
  // print(receive_buffer);
  // print("TX: ");
  // print_character(send_buffer[0]);
  // print_long(send_buffer[0]);
  */
}

void check_for_new_bytes_received() {
  while(serial_get_received_bytes(USB_COMM) != receive_buffer_position) {
    // Process the new byte that has just been received.
    if (receive_buffer[receive_buffer_position] == 0xD) {
      receive_buffer[receive_buffer_position + 1] = '\0';
      // process_received_byte(receive_buffer[receive_buffer_position]);
      process_received_bytes(receive_buffer);
    }

    // Increment receive_buffer_position, but wrap around when it gets to
    // the end of the buffer
    if (receive_buffer_position == sizeof(receive_buffer)-1) {
      receive_buffer_position = 0;
    }
    else {
      receive_buffer_position++;
    }
  }
}
