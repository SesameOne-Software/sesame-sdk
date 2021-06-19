#ifndef SDK_GLOBALS_H
#define SDK_GLOBALS_H

typedef struct {
	float realtime;
	int framecount;
	float abs_frametime;
	float abs_frametime_stddev;
	float curtime;
	float frametime;
	int max_clients;
	int tickcount;
	float tick_interval;
	float interpolation;
    int sim_ticks_this_frame;
    int net_protocol;
    void* save_data;
    bool is_client;
    bool is_remote_client;
    int timestamp_net_base;
    int timestamp_rand_window;
} iglobals;

#endif // !SDK_GLOBALS_H
