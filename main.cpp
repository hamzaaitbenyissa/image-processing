#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "menu.h"
#include "image_processing.h"

// Main application logic
int main()
{
	int menuChoice;
	Dimensions imageDimensions;
	Image image = NULL;
	int imageCount = 0;

	Menu();
	do
	{
		menuChoice = menu();
		handleMenuChoice(menuChoice, image, imageDimensions, imageCount);
	} while (menuChoice != 10);
	return 0;
}
