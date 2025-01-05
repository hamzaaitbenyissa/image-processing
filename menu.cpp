#include "menu.h"
#include "image_processing.h"
#include <unistd.h>

void Menu()
{
    printf("*******************AIT BEN YISSA HAMZA*******************\n\n");
    printf("*  1 ----> Create image                                 *\n");
    printf("*  2 ----> Show image                                   *\n");
    printf("*  3 ----> Frequencies                                  *\n");
    printf("*  4 ----> Negative                                     *\n");
    printf("*  5 ----> Fill                                         *\n");
    printf("*  6 ----> Horizontal Mirror                            *\n");
    printf("*  7 ----> Vertical Mirror                              *\n");
    printf("*  8 ----> Clear screen                                 *\n");
    printf("*  9 ----> Menu                                         *\n");
    printf("* 10 ----> Quit                                         *\n");
    printf("********************************************************\n\n");
}

int menu()
{
    int choice;
    do
    {
        printf("Enter your choice :  ");
        scanf("%d", &choice);
        printf("\n\n");
        if (choice < 1 || choice > 10)
        {
            printf("\tInvalid choice !\n\n");
            sleep(1);
        };
    } while (choice < 1 || choice > 10);
    return choice;
}

void handleMenuChoice(int menuChoice, Image &image, Dimensions &imageDimensions, int &imageCount)
{
    Coordinate coordinate;
    int newPixelColor;
    Image filledImage, mirroredVerticalImage, mirroredHorizontalImage, negativeImage;

    switch (menuChoice)
    {
    case 1:
        getImageDimensions(&imageDimensions.lg, &imageDimensions.h, &imageDimensions.r);
        image = createImage(imageDimensions);
        initImg(image, imageDimensions);
        imageCount++;
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 2:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        displayImage(image, imageDimensions);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 3:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        displayColorFrequencies(image, imageDimensions);
        printf("\n\n");
        sleep(1);
        break;
    case 4:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        negativeImage = createImage(imageDimensions);
        applyNegativeFilter(negativeImage, image, imageDimensions);
        displayImage(negativeImage, imageDimensions);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 5:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        printf("\nPlease enter the coordinates  :  \n\n");
        printf("\tX :  ");
        scanf("%d", &coordinate.x);
        printf("\tY :  ");
        scanf("%d", &coordinate.y);
        printf("New pixel  :  ");
        scanf("%u", &newPixelColor);
        printf("\n\n");
        filledImage = createImage(imageDimensions);
        fillImage(filledImage, image, imageDimensions, coordinate, newPixelColor);
        displayImage(filledImage, imageDimensions);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 6:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        mirroredHorizontalImage = createImage(imageDimensions);
        horizontalMirror(mirroredHorizontalImage, image, imageDimensions);
        displayImage(mirroredHorizontalImage, imageDimensions);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 7:
        if (imageCount == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        mirroredVerticalImage = createImage(imageDimensions);
        verticalMirror(mirroredVerticalImage, image, imageDimensions);
        displayImage(mirroredVerticalImage, imageDimensions);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 8:
        system("cls");
        Menu();
        break;
    case 9:
        Menu();
        break;
    case 10:
        printf("\n\tThank you for using our service.\n\n");
        break;
    default:
        break;
    }
}
