#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned char pixelT; // pixel color
typedef pixelT *ptrPixelT;
typedef ptrPixelT *imgT;
typedef struct
{
	int lg;
	int h;
	int r;
} dms;

// creates an image
imgT createImage(dms d)
{
	imgT img;
	long C = d.lg * d.r; // number of columns
	long L = d.h * d.r;	 // number of lines
	img = (imgT)malloc(L * sizeof(ptrPixelT));
	for (int i = 0; i < L; i++)
		img[i] = (ptrPixelT)malloc(C * sizeof(pixelT));
	return img;
}

// gets image dimensions
void getImageDimensions(int *ptrL, int *ptrH, int *ptrR)
{
	printf("Length : ");
	scanf("%d", ptrL);
	printf("Height : ");
	scanf("%d", ptrH);
	printf("Resolution : ");
	scanf("%d", ptrR);
}

// initializes image pixels with random values
void initImg(imgT im, dms d)
{
	for (int i = 0; i < d.r * d.h; i++)
		for (int j = 0; j < d.r * d.lg; j++)
			im[i][j] = rand() % 256;
}

// displays image
void displayImage(imgT im, dms d)
{
	int L = d.h * d.r, C = d.lg * d.r;

	// Print top border
	printf("  +");
	for (int j = 0; j < C; j++)
	{
		printf("-----");
	}
	printf("+\n");

	for (int i = 0; i < L; i++)
	{
		printf("  |");
		for (int j = 0; j < C; j++)
			printf(" %3u |", im[i][j]);
		printf("\n");

		// Print separator between rows
		printf("  +");
		for (int j = 0; j < C; j++)
		{
			printf("-----");
		}
		printf("+\n");
	}
}

// copies an image
void copyImage(imgT cop, imgT org, dms d)
{
	int L = d.h * d.r, C = d.lg * d.r;
	for (int i = 0; i < L; i++)
		for (int j = 0; j < C; j++)
			cop[i][j] = org[i][j];
}

//--------------------------------------dynamic queue------------------------------

// define a dynamic queue
typedef struct
{
	int x, y;
} coordt1;

typedef struct coordT
{
	int x, y; // pixel coordinates
	struct coordT *suivant;
} coordt;

typedef struct
{
	coordt *tete, *queue;
} filet;

// queue operations:

void enqueue(filet *ptrF, coordt1 px)
{
	coordt *p = (coordt *)malloc(sizeof(coordt));
	p->x = px.x;
	p->y = px.y;
	p->suivant = NULL;
	if (ptrF->queue)
		ptrF->queue->suivant = p;
	else
		ptrF->tete = p;
	ptrF->queue = p;
}

int isEmpty(filet f)
{
	return f.queue == NULL;
}

coordt1 dequeue(filet *ptrF)
{
	coordt1 c = {-1, -1}; // empty queue
	coordt *s;
	if (ptrF->tete)
	{
		s = ptrF->tete;
		c.x = s->x;
		c.y = s->y;
		if (ptrF->tete == ptrF->queue)
			ptrF->tete = ptrF->queue = NULL;
		else
			ptrF->tete = s->suivant;
		free(s);
		s = NULL;
	}
	return c;
}

//-------------------------------------image effects---------------------------------------------

//-----------------------------------transpose an image----------------------------------
void transposeImage(imgT imgtr /*destination*/, imgT img /*source*/, dms d)
{
	for (int i = 0; i < d.h * d.r; i++)
		for (int j = 0; j < d.lg * d.r; j++)
			imgtr[i][j] = img[j][i];
}

// Horizontal mirror effect
void horizontalMirror(imgT mir /*destination*/, imgT img /*source*/, dms d)
{
	int L = d.h * d.r, C = d.lg * d.r;
	int i, ii;
	copyImage(mir, img, d);
	for (i = 0, ii = L - 1; i < ii; i++, ii--)
	{
		ptrPixelT temp = img[i];
		mir[i] = mir[ii];
		mir[ii] = temp;
	}
}

// Vertical mirror effect
void verticalMirror(imgT mir /*destination*/, imgT img /*source*/, dms d)
{
	dms dt = {d.h, d.lg, d.r};
	imgT img_temp1 = createImage(dt);
	imgT img_temp2 = createImage(dt);
	transposeImage(img_temp2, img, dt);
	horizontalMirror(img_temp1, img_temp2, dt);
	transposeImage(mir, img_temp1, d);
}

// fill the image
void fillImage(imgT im /*destination*/, imgT img /*source*/, dms d, coordt1 px, pixelT cn)
{
	copyImage(im, img, d);
	filet f = {NULL, NULL}; // empty queue;
	coordt1 px2, px1;
	enqueue(&f, px);
	pixelT c0 = im[px.x][px.y];
	while (!isEmpty(f))
	{
		px1 = dequeue(&f);
		im[px1.x][px1.y] = cn;
		if (px1.x > 0 && im[px1.x - 1][px1.y] == c0)
		{
			px2.x = px1.x - 1;
			px2.y = px1.y;
			enqueue(&f, px2);
		}
		if (px1.y > 0 && im[px1.x][px1.y - 1] == c0)
		{
			px2.x = px1.x;
			px2.y = px1.y - 1;
			enqueue(&f, px2);
		}
		if (px1.x < d.h * d.r - 1 && im[px1.x + 1][px1.y] == c0)
		{
			px2.x = px1.x + 1;
			px2.y = px1.y;
			enqueue(&f, px2);
		}
		if (px1.y < d.lg * d.r - 1 && im[px1.x][px1.y + 1] == c0)
		{
			px2.x = px1.x;
			px2.y = px1.y + 1;
			enqueue(&f, px2);
		}
	}
}

//-----------------------------------sort an image----------------------------------
// find the min of a line
int findMinInRow(imgT M, dms d, int i0, int j0)
{
	int L = d.h * d.r, C = d.lg * d.r;
	pixelT min = M[i0][j0];
	int i, j, indice = i0;
	for (i = i0; i < L; i++)
	{
		if (i == i0)
		{
			for (j = j0; j < C; j++)
				if (min > M[i][j])
				{
					indice = i;
					min = M[i][j];
				}
		}
		else
		{
			for (j = 0; j < C; j++)
				if (min > M[i][j])
				{
					indice = i;
					min = M[i][j];
				}
		}
	}
	return indice;
}

// find the min of a column
int findMinInColumn(imgT M, dms d, int i0, int j0)
{
	int L = d.h * d.r, C = d.lg * d.r;
	pixelT min = M[i0][j0];
	int i, j, indice = j0;
	for (i = i0; i < L; i++)
	{
		if (i == i0)
		{
			for (j = j0; j < C; j++)
				if (min > M[i][j])
				{
					indice = j;
					min = M[i][j];
				}
		}
		else
		{
			for (j = 0; j < C; j++)
				if (min > M[i][j])
				{
					indice = j;
					min = M[i][j];
				}
		}
	}
	return indice;
}

// swap two pixels
void swapPixels(imgT M, int i0, int j0, int i1, int j1)
{
	pixelT aux;
	aux = M[i0][j0];
	M[i0][j0] = M[i1][j1];
	M[i1][j1] = aux;
}

// sorting algorithm
void sortImage(imgT M, dms d)
{
	int L = d.h * d.r, C = d.lg * d.r;
	int i, j;
	for (i = 0; i < L; i++)
		for (j = 0; j < C; j++)
			swapPixels(M, i, j, findMinInRow(M, d, i, j), findMinInColumn(M, d, i, j));
}

//------------------------------------dominant color-------------------------------
// know the number of distinct colors in an image
int countDistinctColors(imgT M, dms d)
{
	imgT MC = createImage(d);
	copyImage(MC, M, d);
	int L = d.h * d.r, C = d.lg * d.r;
	sortImage(MC, d);
	int s = 1, i, j;
	for (i = 0; i < L; i++)
	{
		for (j = 0; j < C - 1; j++)
			if (MC[i][j] != MC[i][j + 1])
				s++;
		if (i != L - 1 && MC[i][C - 1] != MC[i + 1][0])
			s++;
	}
	return s;
}

// get the colors of an image
ptrPixelT getColors(imgT img, dms d)
{
	int L = d.h * d.r, C = d.lg * d.r;
	ptrPixelT clr = (ptrPixelT)malloc(sizeof(pixelT) * countDistinctColors(img, d));
	imgT MC = createImage(d);
	copyImage(MC, img, d);
	sortImage(MC, d);
	int s = 1, i, j;
	clr[0] = MC[0][0];
	for (i = 0; i < L; i++)
	{
		for (j = 0; j < C - 1; j++)
			if (MC[i][j] != MC[i][j + 1])
			{
				clr[s] = MC[i][j + 1];
				s++;
			}
		if (i != L - 1 && MC[i][C - 1] != MC[i + 1][0])
		{
			clr[s] = MC[i + 1][0];
			s++;
		}
	}
	return clr;
}

// frequency of a color in an image
float getColorFrequency(imgT img, dms d, pixelT c)
{
	int s = 0;
	float t;
	for (int i = 0; i < d.r * d.h; i++)
	{
		for (int j = 0; j < d.r * d.lg; j++)
			if (img[i][j] == c)
				s++;
	}
	t = float(s * 100) / float(d.lg * d.h * d.r * d.r);
	return t;
}

// display colors of an image and their frequencies
void displayColorFrequencies(imgT img, dms d)
{
	ptrPixelT m = getColors(img, d);
	float max = getColorFrequency(img, d, *(m));
	int indice_max = 0;
	for (int i = 0; i < countDistinctColors(img, d); i++)
	{
		printf("color%d : %u----------->%.2f %%\n\n", i, *(m + i), getColorFrequency(img, d, *(m + i)));
		if (max < getColorFrequency(img, d, *(m + i)))
		{
			max = getColorFrequency(img, d, *(m + i));
			indice_max = i;
		}
	}
	printf("\n\nThe dominant color is :%4u", *(m + indice_max));
}
void applyNegativeFilter(imgT neg /*destination*/, imgT img /*source*/, dms d)
{
	for (int i = 0; i < d.r * d.h; i++)
		for (int j = 0; j < d.r * d.lg; j++)
		{
			if (img[i][j] < 128)
				neg[i][j] = 0;
			else
				neg[i][j] = 1;
		}
}

// Displays the menu of options
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

// Gets the user's menu choice
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

// Function to handle menu choices
void handleMenuChoice(int choice, imgT &img, dms &d, int &k)
{
	coordt1 crd;
	int cn;
	imgT img_remplissage, img_miroir_V, img_miroir_H, img_negatif;

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

// Main application logic
int main()
{
	int choice;
	dms d;
	imgT img = NULL;
	int k = 0;

	Menu();
	do
	{
		choice = menu();
		handleMenuChoice(choice, img, d, k);
	} while (choice != 10);
	return 0;
}
