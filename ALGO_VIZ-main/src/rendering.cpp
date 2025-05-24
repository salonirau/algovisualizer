#include "rendering.h"
#include "utilities.h"
#include <mutex>
#include <algorithm>

SDL_Window* windows[6] = {nullptr};
SDL_Renderer* renderers[6] = {nullptr};
TTF_Font* font = nullptr;

std::mutex render_mtx;

bool init(int options[], int count) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("arial.ttf", 13);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    std::string titles[] = {"Selection Sort Visualizer", "Insertion Sort Visualizer", "Bubble Sort Visualizer", "Merge Sort Visualizer", "Quick Sort Visualizer", "Heap Sort Visualizer"};

    for (int k = 0; k < count; ++k) {
        int opt = options[k];
        if (opt < 1 || opt > 6) {
            std::cerr << "Invalid option!" << std::endl;
            return false;
        }

        int i = opt - 1;
        int posX = 20;
        int posY = 40 + (i % 3) * (WINDOW_HEIGHT + 30);

        windows[i] = SDL_CreateWindow(titles[i].c_str(), posX, posY, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (!windows[i]) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        renderers[i] = SDL_CreateRenderer(windows[i], -1, SDL_RENDERER_ACCELERATED);
        if (!renderers[i]) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(windows[i]);
            SDL_Quit();
            return false;
        }
    }
    return true;
}

void close() {
    for (int i = 0; i < 6; ++i) {
        if (renderers[i]) SDL_DestroyRenderer(renderers[i]);
        if (windows[i]) SDL_DestroyWindow(windows[i]);
    }
    TTF_CloseFont(font);
    font = nullptr;
    TTF_Quit();
    SDL_Quit();
}

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255};  // White color for text
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surfaceMessage) {
        std::cerr << "Failed to create surface for text! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    if (!message) {
        std::cerr << "Failed to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surfaceMessage);
        return;
    }

    SDL_Rect messageRect = {x, y, surfaceMessage->w, surfaceMessage->h};

    SDL_RenderCopy(renderer, message, nullptr, &messageRect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void renderSort(SDL_Renderer* renderer, const std::vector<int>& arr, size_t currentIndex, size_t secondIndex, const std::string& mode) {
    std::lock_guard<std::mutex> lock(render_mtx);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    int numElements = arr.size();
    int barWidth = (WINDOW_WIDTH - (BAR_GAP * (numElements - 1))) / numElements;

    for (size_t i = 0; i < arr.size(); ++i) {
        int height = (arr[i] * (WINDOW_HEIGHT - 40)) / 100;
        SDL_Rect bar = {static_cast<int>(i * (barWidth + BAR_GAP)), WINDOW_HEIGHT - height - 30, barWidth, height};

        if (mode == "update") {
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        } else if (mode == "selection") {
            if (i == currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Highlighting the current index
            } else if (i == secondIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Highlighting the compared index
            } else {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
            }
        } else if (mode == "insertion") {
             if (i < currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Sorted elements
            } else if (i == currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Highlighting the key being moved
            } else if (i == secondIndex) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Highlighting the index being moved to
            } else {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xA5, 0x00, 0xFF); // Other elements
            }
        }
        else if (mode == "merge") {
            if (i < currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF); // Merged elements
            } else if (i == secondIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Highlighting the compared index
            } else {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xA5, 0x00, 0xFF);
            }
        } else if (mode == "quick") {
            if (i == currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Current element
            } else if (i == secondIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Pivot
            } else {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xA5, 0x00, 0xFF); // Other elements
            }
        } else if (mode == "heap") {
            if (i == currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Current node
            } else {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x77, 0xFF); // Other elements
            }
        } else {
            if (i == currentIndex) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF); // Current element
            } else if (i == secondIndex) {
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF); // Compared element
            } else {
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); // Sorted elements
            }
        }

        SDL_RenderFillRect(renderer, &bar);

        int textYPos = std::max(WINDOW_HEIGHT - height - 30 - 20, 0);
        renderText(renderer, std::to_string(arr[i]), static_cast<int>(i * (barWidth + BAR_GAP)), textYPos);
    }

    SDL_RenderPresent(renderer);
}
