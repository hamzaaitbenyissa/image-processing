#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>

// Define types
typedef unsigned char pixelT; // pixel color
typedef pixelT *ptrPixelT;
typedef ptrPixelT *imgT;
typedef struct
{
    int lg;
    int h;
    int r;
} dms;

// Function declarations for creating, initializing, displaying, and copying images
imgT createImage(dms d);
void getImageDimensions(int *ptrL, int *ptrH, int *ptrR);
void initImg(imgT im, dms d);
void displayImage(imgT im, dms d);
void copyImage(imgT cop, imgT org, dms d);

// Function declarations for image effects
void transposeImage(imgT imgtr, imgT img, dms d);
void horizontalMirror(imgT mir, imgT img, dms d);
void verticalMirror(imgT mir, imgT img, dms d);
void fillImage(imgT im, imgT img, dms d, coordt1 px, pixelT cn);
void applyNegativeFilter(imgT neg, imgT img, dms d);

// Function declarations for color frequency analysis and sorting
int findMinInRow(imgT M, dms d, int i0, int j0);
int findMinInColumn(imgT M, dms d, int i0, int j0);
void swapPixels(imgT M, int i0, int j0, int i1, int j1);
void sortImage(imgT M, dms d);
int countDistinctColors(imgT M, dms d);
ptrPixelT getColors(imgT img, dms d);
float getColorFrequency(imgT img, dms d, pixelT c);
void displayColorFrequencies(imgT img, dms d);

// Function declarations for dynamic queue operations
void enqueue(filet *ptrF, coordt1 px);
int isEmpty(filet f);
coordt1 dequeue(filet *ptrF);

#endif // IMAGE_PROCESSING_H
