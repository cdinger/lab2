#include <pololu/orangutan.h>

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

// process_received_byte: Responds to a byte that has been received on
// USB_COMM.  If you are writing your own serial program, you can
// replace all the code in this function with your own custom behaviors.
void process_received_byte(char byte) {
  clear();
  print("RX: ");
  print_character(byte);
  lcd_goto_xy(0, 1); // go to start of second LCD row

  switch(byte) {
    case 'L':
    case 'l':
      break;

    // If any other character is received, change its capitalization and
    // send it back.
    default:
      wait_for_sending_to_finish();
      send_buffer[0] = byte; // ^ 0x20;
      serial_send(USB_COMM, send_buffer, 1);
      print("TX: ");
      print_character(send_buffer[0]);
      break;
  }
}

void check_for_new_bytes_received() {
  while(serial_get_received_bytes(USB_COMM) != receive_buffer_position) {
    // Process the new byte that has just been received.
    process_received_byte(receive_buffer[receive_buffer_position]);

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

// void send_serial(char bytes[]) {
void send_serial(const char * bytes) {
  char b[strlen(bytes)];
  int i;
  for (i=0; i < strlen(bytes); i++) {
    b[i] = bytes[i];
  }

  wait_for_sending_to_finish();
  // send_buffer[0] = byte;
  // serial_send(USB_COMM, send_buffer, 1);
  serial_send(USB_COMM, b, strlen(bytes));
}

