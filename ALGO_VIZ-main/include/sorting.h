#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <SDL2/SDL.h>

void selectionSort(std::vector<int>& arr, SDL_Renderer* renderer);
void insertionSort(std::vector<int>& arr, SDL_Renderer* renderer);
void bubbleSort(std::vector<int>& arr, SDL_Renderer* renderer);
void mergeSort(std::vector<int>& arr, int left, int right, SDL_Renderer* renderer);
void quickSort(std::vector<int>& arr, int low, int high, SDL_Renderer* renderer);
void heapSort(std::vector<int>& arr, SDL_Renderer* renderer);

void executeSorting(int option);

#endif // SORTING_H
