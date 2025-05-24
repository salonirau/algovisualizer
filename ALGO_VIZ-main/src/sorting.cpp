#include "sorting.h"
#include "rendering.h"
#include "events.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include<iostream>
extern bool quit;
extern bool paused;
extern int delay;
extern std::mutex mtx;
extern std::condition_variable cv;

void selectionSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.size(); ++j) {
            handleEvents();
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (quit) return;
            }
            waitForResume();
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
            renderSort(renderer, arr, j, minIndex, "selection");
            SDL_Delay(delay);
        }
        std::swap(arr[i], arr[minIndex]);
        renderSort(renderer, arr, i, minIndex, "selection");
        SDL_Delay(delay);
    }
}

void insertionSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        size_t j = i - 1;

        while (j < arr.size() && arr[j] > key) {
            handleEvents();
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (quit) return;
            }
            waitForResume();
            arr[j + 1] = arr[j];
            renderSort(renderer, arr, i, j+1, "insertion"); // Pass i as currentIndex and j as secondIndex
            j--;
            SDL_Delay(delay);
        }
        arr[j + 1] = key;
        renderSort(renderer, arr, j + 1, i, "insertion"); // Pass j + 1 as currentIndex and i as secondIndex
        SDL_Delay(delay);
    }
}

void bubbleSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        for (size_t j = 0; j < arr.size() - i - 1; ++j) {
            handleEvents();
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (quit) return;  
            }
            waitForResume(); 
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
            renderSort(renderer, arr, j, j + 1, "bubble");
            SDL_Delay(delay);
        }
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right, SDL_Renderer* renderer) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        handleEvents();
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (quit) return;
        }
        waitForResume();
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        renderSort(renderer, arr, k - 1, -1, "merge");
        SDL_Delay(delay); // Add delay to slow down the merge visualization
    }

    while (i < n1) {
        handleEvents();
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (quit) return;
        }
        waitForResume();
        arr[k++] = L[i++];
        renderSort(renderer, arr, k - 1, -1, "merge");
        SDL_Delay(delay); // Add delay to slow down the merge visualization
    }
    while (j < n2) {
        handleEvents();
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (quit) return;
        }
        waitForResume();
        arr[k++] = R[j++];
        renderSort(renderer, arr, k - 1, -1, "merge");
        SDL_Delay(delay); // Add delay to slow down the merge visualization
    }
}

void mergeSort(std::vector<int>& arr, int left, int right, SDL_Renderer* renderer) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, renderer);
        mergeSort(arr, mid + 1, right, renderer);
        merge(arr, left, mid, right, renderer);
    }
}

void quickSort(std::vector<int>& arr, int low, int high, SDL_Renderer* renderer) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; ++j) {
            handleEvents();
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (quit) return;  
            }
            waitForResume(); 
            if (arr[j] < pivot) {
                ++i;
                std::swap(arr[i], arr[j]);
                renderSort(renderer, arr, i, j, "quick");
                SDL_Delay(delay);
            }
            renderSort(renderer, arr, j, high, "quick");
            SDL_Delay(delay);
        }
        std::swap(arr[i + 1], arr[high]);
        renderSort(renderer, arr, i + 1, high, "quick");
        SDL_Delay(delay);
        
        quickSort(arr, low, i, renderer);
        quickSort(arr, i + 2, high, renderer);
    }
}

void heapify(std::vector<int>& arr, int n, int i, SDL_Renderer* renderer) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        renderSort(renderer, arr, largest, i, "heap");
        SDL_Delay(delay); // Use the new delay for heap sort
        handleEvents();
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (quit) return;  
        }
        waitForResume();
        heapify(arr, n, largest, renderer);
    }
}

void heapSort(std::vector<int>& arr, SDL_Renderer* renderer) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, renderer);
        renderSort(renderer, arr, i, -1, "heap");
        SDL_Delay(delay); // Use the new delay here as well
    }
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        renderSort(renderer, arr, i, -1, "heap");
        SDL_Delay(delay); // Use the new delay here as well
        heapify(arr, i, 0, renderer);
    }
}

void executeSorting(int option) {
    std::vector<int> arr(70);
    for (int i = 0; i < 70; ++i) {
        arr[i] = rand() % 100;
    }

    switch (option) {
        case 1:
            selectionSort(arr, renderers[0]);
            break;
        case 2:
            insertionSort(arr, renderers[1]);
            break;
        case 3:
            bubbleSort(arr, renderers[2]);
            break;
        case 4:
            mergeSort(arr, 0, arr.size() - 1, renderers[3]);
            break;
        case 5:
            quickSort(arr, 0, arr.size() - 1, renderers[4]);
            break;
        case 6:
            heapSort(arr, renderers[5]);
            break;
        default:
            std::cerr << "Invalid option!" << std::endl;
            break;
    }
}
