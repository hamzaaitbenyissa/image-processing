#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "menu.h"
#include "image_processing.h"

// Main application logic
int main()
{
	int choice;
	Dimensions d;
	Image img = NULL;
	int k = 0;

	Menu();
	do
	{
		choice = menu();
		handleMenuChoice(choice, img, d, k);
	} while (choice != 10);
	return 0;
}
