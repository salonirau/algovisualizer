#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 230;
const int BAR_GAP = 5;

extern SDL_Window* windows[6];
extern SDL_Renderer* renderers[6];
extern TTF_Font* font;

bool init(int options[], int count);
void close();
void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y);
void renderSort(SDL_Renderer* renderer, const std::vector<int>& arr, size_t currentIndex, size_t secondIndex, const std::string& mode);

#endif // RENDERING_H
