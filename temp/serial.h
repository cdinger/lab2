char receive_buffer[32];
unsigned char receive_buffer_position;
char send_buffer[32];
int G_logging_enabled;

void print_usb(const char *buffer);
void show_menu();
void initialize_serial();
void toggle_logging();
void wait_for_sending_to_finish();
void process_received_byte(char byte);
void process_received_bytes(const char * command);
void check_for_new_bytes_received();
