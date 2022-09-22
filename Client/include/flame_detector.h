#ifndef FLAME_DETECTOR_H
#define FLAME_DETECTOR_H

void handle_detector();
void init_detector();
void mqtt_send_msg_flame(int state);

#endif