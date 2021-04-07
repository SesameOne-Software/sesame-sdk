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
} iglobals;

#endif // !SDK_GLOBALS_H