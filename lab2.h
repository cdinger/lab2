int G_ms_ticks;
int G_release_pd;
int G_current_speed;
int G_max_speed;
int G_previous_counts;
int G_previous_T;
int G_Pm;
int G_Pr;
volatile int G_Kp;
volatile int G_Kd;
int G_Vm;
int G_T;
int G_logging_enabled;
int G_relative_degrees;

void toggle_logging();
void debug(msg);
void initialize_pd_timer();
void initialize_pd_controller();
int degrees_in_wheel_ticks(int degrees);
void pd_control(int relative_degrees);
void interpolate_trajetory();
int main();
