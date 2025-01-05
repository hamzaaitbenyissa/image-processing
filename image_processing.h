#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>

// Define types
typedef unsigned char Pixel; // pixel color
typedef Pixel *PixelPointer;
typedef PixelPointer *Image;
typedef struct
{
    int x, y;
} Coordinate;
typedef struct coordT
{
    int x, y; // pixel coordinates
    struct coordT *suivant;
} CoordinateNode;

typedef struct
{
    CoordinateNode *tete, *queue;
} Queue;
typedef struct
{
    int lg;
    int h;
    int r;
} Dimensions;

// Function declarations for creating, initializing, displaying, and copying images
Image createImage(Dimensions d);
void getImageDimensions(int *ptrL, int *ptrH, int *ptrR);
void initImg(Image im, Dimensions d);
void displayImage(Image im, Dimensions d);
void copyImage(Image cop, Image org, Dimensions d);

// Function declarations for image effects
void transposeImage(Image imgtr, Image img, Dimensions d);
void horizontalMirror(Image mir, Image img, Dimensions d);
void verticalMirror(Image mir, Image img, Dimensions d);
void fillImage(Image im, Image img, Dimensions d, Coordinate px, Pixel cn);
void applyNegativeFilter(Image neg, Image img, Dimensions d);

// Function declarations for color frequency analysis and sorting
int findMinInRow(Image M, Dimensions d, int i0, int j0);
int findMinInColumn(Image M, Dimensions d, int i0, int j0);
void swapPixels(Image M, int i0, int j0, int i1, int j1);
void sortImage(Image M, Dimensions d);
int countDistinctColors(Image M, Dimensions d);
PixelPointer getColors(Image img, Dimensions d);
float getColorFrequency(Image img, Dimensions d, Pixel c);
void displayColorFrequencies(Image img, Dimensions d);

// Function declarations for dynamic queue operations
void enqueue(Queue *ptrF, Coordinate px);
int isEmpty(Queue f);
Coordinate dequeue(Queue *ptrF);

#endif // IMAGE_PROCESSING_H
