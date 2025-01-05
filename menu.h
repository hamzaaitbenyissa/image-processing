#ifndef MENU_H
#define MENU_H

#include "image_processing.h"

// Function declarations for menu operations
void Menu();
int menu();
void handleMenuChoice(int choice, Image &img, Dimensions &d, int &k);

#endif // MENU_H
