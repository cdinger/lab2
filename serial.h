char receive_buffer[32];
unsigned char receive_buffer_position;
char send_buffer[32];

void wait_for_sending_to_finish();
void process_received_byte(char byte);
void check_for_new_bytes_received();
void send_serial(char * bytes);
