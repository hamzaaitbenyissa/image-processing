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
    int c;
    do
    {
        printf("Enter your choice :  ");
        scanf("%d", &c);
        printf("\n\n");
        if (c < 1 || c > 10)
        {
            printf("\tInvalid choice !\n\n");
            sleep(1);
        };
    } while (c < 1 || c > 10);
    return c;
}

void handleMenuChoice(int choice, Image &img, Dimensions &d, int &k)
{
    Coordinate crd;
    int cn;
    Image img_remplissage, img_miroir_V, img_miroir_H, img_negatif;

    switch (choice)
    {
    case 1:
        getImageDimensions(&d.lg, &d.h, &d.r);
        img = createImage(d);
        initImg(img, d);
        k++;
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 2:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        displayImage(img, d);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 3:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        displayColorFrequencies(img, d);
        printf("\n\n");
        sleep(1);
        break;
    case 4:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        img_negatif = createImage(d);
        applyNegativeFilter(img_negatif, img, d);
        displayImage(img_negatif, d);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 5:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        printf("\nPlease enter the coordinates  :  \n\n");
        printf("\tX :  ");
        scanf("%d", &crd.x);
        printf("\tY :  ");
        scanf("%d", &crd.y);
        printf("New pixel  :  ");
        scanf("%u", &cn);
        printf("\n\n");
        img_remplissage = createImage(d);
        fillImage(img_remplissage, img, d, crd, cn);
        displayImage(img_remplissage, d);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 6:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        img_miroir_H = createImage(d);
        horizontalMirror(img_miroir_H, img, d);
        displayImage(img_miroir_H, d);
        printf("\n\n\t Operation completed successfully.  \n\n");
        sleep(1);
        break;
    case 7:
        if (k == 0)
        {
            printf("\nPlease create an image first!\n\n");
            break;
        }
        img_miroir_V = createImage(d);
        verticalMirror(img_miroir_V, img, d);
        displayImage(img_miroir_V, d);
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
