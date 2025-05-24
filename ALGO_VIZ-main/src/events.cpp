#include "events.h"
#include "utilities.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

// Define the variables in events.cpp
bool quit = false;
bool paused = false;
int delay = 100;

std::mutex mtx;
std::condition_variable cv;

void handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            std::lock_guard<std::mutex> lock(mtx);
            quit = true;
            cv.notify_all();
        } else if (e.type == SDL_KEYDOWN) {
            std::lock_guard<std::mutex> lock(mtx);
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
                cv.notify_all();
            } else if (e.key.keysym.sym == SDLK_p) {
                paused = !paused;
                if (paused) {
                    clearScreen();
                    std::cout << RED << "Paused. Press 'P' to resume." << RESET << "\n";
                } else {
                    cv.notify_all();
                }
            } else if (e.key.keysym.sym == SDLK_RIGHT) {
                if (delay > 10) {
                    clearScreen();
                    delay -= 10;
                    std::cout << GREEN << "Speed increased. Delay: " << delay << "ms" << RESET << "\n";
                }
            } else if (e.key.keysym.sym == SDLK_LEFT) {
                clearScreen();
                delay += 10;
                std::cout << YELLOW << "Speed decreased. Delay: " << delay << "ms" << RESET << "\n";
            }
        }
    }
}

void waitForResume() {
    std::unique_lock<std::mutex> lock(mtx);
    while (paused && !quit) {
        cv.wait(lock);
        handleEvents(); 
    }
    if (quit) {
        lock.unlock();
    }
}
