#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>
#include <algorithm>
#include <limits>

#include "rendering.h"
#include "sorting.h"
#include "utilities.h"
#include "events.h"  // Include events.h to access quit and delay

const int MAX_VISUALIZATIONS = 3;

void showWelcomeMessage() {
    clearScreen();
    std::cout << CYAN << "==============================" << RESET << std::endl;
    std::cout << YELLOW << "  Welcome to the Sorting Visualizer!" << RESET << std::endl;
    std::cout << CYAN << "==============================" << RESET << std::endl;
    std::cout << GREEN << "This sorting visualizer visualizes multiple sorting algorithms:" << RESET << std::endl;
    std::cout << GREEN << " - Insertion Sort" << RESET << std::endl;
    std::cout << GREEN << " - Selection Sort" << RESET << std::endl;
    std::cout << GREEN << " - Merge Sort" << RESET << std::endl;
    std::cout << GREEN << " - Bubble Sort" << RESET << std::endl;
    std::cout << GREEN << " - Quick Sort" << RESET << std::endl;
    std::cout << GREEN << " - Heap Sort" << RESET << std::endl;
    std::cout << BLUE << "\nYou can speed up or slow down using the left and right arrow keys, respectively." << RESET << std::endl;
    std::cout << BLUE << "Press 'P' to pause and 'ESC' to quit the window." << RESET << std::endl;
    std::cout << RED << "\nCaution: Pressing multiple keys during multiple visualizations may cause the program to crash!" << RESET << std::endl;
    std::cout << CYAN << "\nPress 'Y' to continue to the main menu..." << RESET << std::endl;

    char response;
    std::cin >> response;
    while (response != 'Y' && response != 'y') {
        std::cout << RED << "Invalid input! Please press 'Y' to continue." << RESET << std::endl;
        std::cin >> response;
    }
}

void showmenu() {
    std::cout << CYAN << "=====================" << RESET << std::endl;
    std::cout << YELLOW << "  Main Menu\n" << RESET;
    std::cout << CYAN << "=====================" << RESET << std::endl;
    std::cout << GREEN << "1. One Visualization\n" << RESET;
    std::cout << GREEN << "2. Multiple Visualizations\n" << RESET;
    std::cout << GREEN << "3. Change Speed\n" << RESET;
    std::cout << GREEN << "4. Exit\n" << RESET;
    std::cout << BLUE << "Enter your choice: " << RESET;
}

void showSingleVisualizationMenu() {
    clearScreen();
    int sortOption;
    std::cout << "Select sorting algorithm to visualize:\n";
    std::cout << GREEN << "1. Selection Sort\n" << RESET;
    std::cout << GREEN << "2. Insertion Sort\n" << RESET;
    std::cout << GREEN << "3. Bubble Sort\n" << RESET;
    std::cout << GREEN << "4. Merge Sort\n" << RESET;
    std::cout << GREEN << "5. Quick Sort\n" << RESET;
    std::cout << GREEN << "6. Heap Sort\n" << RESET;
    std::cout << BLUE << "Enter your choice (1-6): " << RESET;
    std::cin >> sortOption;

    if (std::cin.fail() || sortOption < 1 || sortOption > 6) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << RED << "Invalid choice! Please enter a number between 1 and 6." << RESET << "\n";
        return;
    }

    int options[] = {sortOption};
    if (!init(options, 1)) {
        return;
    }

    std::thread sortingThread(executeSorting, sortOption);
    while (!quit) {
        handleEvents();
        SDL_Delay(10);
    }
    sortingThread.join();
    close();
    quit = false;
}

void showMultipleVisualizationsMenu() {
    clearScreen();
    int numSorts;
    std::cout << "How many sorting algorithms to visualize (1-" << MAX_VISUALIZATIONS << "): ";
    std::cin >> numSorts;

    if (std::cin.fail() || numSorts < 1 || numSorts > MAX_VISUALIZATIONS) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << RED << "Invalid number of visualizations! Please enter a number between 1 and " << MAX_VISUALIZATIONS << "." << RESET << "\n";
        return;
    }
    clearScreen();

    int options[6];
    std::cout << "Select the sorting algorithms to visualize:\n";
    std::cout << GREEN << "1. Selection Sort\n" << RESET;
    std::cout << GREEN << "2. Insertion Sort\n" << RESET;
    std::cout << GREEN << "3. Bubble Sort\n" << RESET;
    std::cout << GREEN << "4. Merge Sort\n" << RESET;
    std::cout << GREEN << "5. Quick Sort\n" << RESET;
    std::cout << GREEN << "6. Heap Sort\n" << RESET;
    for (int i = 0; i < numSorts; ++i) {
        std::cout << BLUE << "Enter choice " << (i + 1) << " (1-6): " << RESET;
        std::cin >> options[i];

        if (std::cin.fail() || options[i] < 1 || options[i] > 6) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << RED << "Invalid choice! Please enter a number between 1 and 6." << RESET << "\n";
            --i; 
        }
    }

    if (!init(options, numSorts)) {
        return;
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numSorts; ++i) {
        threads.push_back(std::thread(executeSorting, options[i]));
    }

    while (!quit) {
        handleEvents();
        SDL_Delay(10);
    }

    for (auto& thread : threads) {
        if (thread.joinable()) thread.join();
    }
    close();
    quit = false;
}

void changeSpeed() {
    clearScreen();
    int speedOption;
    std::cout << CYAN << "Select speed:\n" << RESET;
    std::cout << GREEN << "1. Slow\n" << RESET;
    std::cout << GREEN << "2. Medium\n" << RESET;
    std::cout << GREEN << "3. Fast\n" << RESET;
    std::cout << BLUE << "Enter your choice: " << RESET;
    std::cin >> speedOption;

    switch (speedOption) {
        case 1:
            delay = 300;
            break;
        case 2:
            delay = 100;
            break;
        case 3:
            delay = 50;
            break;
        default:
            std::cerr << RED << "Invalid choice! Using default speed (Medium).\n" << RESET;
            delay = 100;
            break;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    showWelcomeMessage();
    int menuChoice;
    bool running = true;

    while (running) {
        clearScreen();
        showmenu();
        std::cin >> menuChoice;

        if (std::cin.fail() || menuChoice < 1 || menuChoice > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << RED << "Invalid choice! Please enter a number between 1 and 4." << RESET << "\n";
            continue;
        }

        switch (menuChoice) {
            case 1:
                showSingleVisualizationMenu();
                break;
            case 2:
                showMultipleVisualizationsMenu();
                break;
            case 3:
                changeSpeed();
                break;
            case 4:
                running = false;
                break;
            default:
                std::cerr << RED << "Invalid choice! Please enter a number between 1 and 4." << RESET << "\n";
                break;
        }
    }

    close();
    return 0;
}
