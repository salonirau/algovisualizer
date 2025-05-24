#ifndef EVENTS_H
#define EVENTS_H

#include <condition_variable>

extern bool quit;  // Declare quit as extern to make it available across files
extern bool paused;
extern int delay;

extern std::mutex mtx;
extern std::condition_variable cv;

void handleEvents();
void waitForResume();

#endif // EVENTS_H
