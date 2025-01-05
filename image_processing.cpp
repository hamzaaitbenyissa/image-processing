#include "image_processing.h"

// creates an image
Image createImage(Dimensions d)
{
    Image image;
    long numCols = d.lg * d.r; // number of columns
    long numRows = d.h * d.r;  // number of lines
    image = (Image)malloc(numRows * sizeof(PixelPointer));
    for (int i = 0; i < numRows; i++)
        image[i] = (PixelPointer)malloc(numCols * sizeof(Pixel));
    return image;
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
void initImg(Image image, Dimensions d)
{
    for (int i = 0; i < d.r * d.h; i++)
        for (int j = 0; j < d.r * d.lg; j++)
            image[i][j] = rand() % 256;
}

// displays image
void displayImage(Image image, Dimensions d)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;

    // Print top border
    printf("  +");
    for (int j = 0; j < numCols; j++)
    {
        printf("-----");
    }
    printf("+\n");

    for (int i = 0; i < numRows; i++)
    {
        printf("  |");
        for (int j = 0; j < numCols; j++)
            printf(" %3u |", image[i][j]);
        printf("\n");

        // Print separator between rows
        printf("  +");
        for (int j = 0; j < numCols; j++)
        {
            printf("-----");
        }
        printf("+\n");
    }
}

// copies an image
void copyImage(Image destImage, Image srcImage, Dimensions d)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    for (int i = 0; i < numRows; i++)
        for (int j = 0; j < numCols; j++)
            destImage[i][j] = srcImage[i][j];
}

//--------------------------------------dynamic queue------------------------------

// queue operations:

void enqueue(Queue *ptrF, Coordinate px)
{
    CoordinateNode *p = (CoordinateNode *)malloc(sizeof(CoordinateNode));
    p->x = px.x;
    p->y = px.y;
    p->suivant = NULL;
    if (ptrF->queue)
        ptrF->queue->suivant = p;
    else
        ptrF->tete = p;
    ptrF->queue = p;
}

int isEmpty(Queue f)
{
    return f.queue == NULL;
}

Coordinate dequeue(Queue *ptrF)
{
    Coordinate c = {-1, -1}; // empty queue
    CoordinateNode *s;
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
void transposeImage(Image transposedImage /*destination*/, Image originalImage /*source*/, Dimensions d)
{
    for (int i = 0; i < d.h * d.r; i++)
        for (int j = 0; j < d.lg * d.r; j++)
            transposedImage[i][j] = originalImage[j][i];
}

// Horizontal mirror effect
void horizontalMirror(Image mirroredImage /*destination*/, Image originalImage /*source*/, Dimensions d)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    int i, ii;
    copyImage(mirroredImage, originalImage, d);
    for (i = 0, ii = numRows - 1; i < ii; i++, ii--)
    {
        PixelPointer temp = originalImage[i];
        mirroredImage[i] = mirroredImage[ii];
        mirroredImage[ii] = temp;
    }
}

// Vertical mirror effect
void verticalMirror(Image mirroredImage /*destination*/, Image originalImage /*source*/, Dimensions d)
{
    Dimensions dt = {d.h, d.lg, d.r};
    Image tempImage1 = createImage(dt);
    Image tempImage2 = createImage(dt);
    transposeImage(tempImage2, originalImage, dt);
    horizontalMirror(tempImage1, tempImage2, dt);
    transposeImage(mirroredImage, tempImage1, d);
}

// fill the image
void fillImage(Image filledImage /*destination*/, Image originalImage /*source*/, Dimensions d, Coordinate startCoordinate, Pixel newColor)
{
    copyImage(filledImage, originalImage, d);
    Queue queue = {NULL, NULL}; // empty queue;
    Coordinate currentCoordinate, nextCoordinate;
    enqueue(&queue, startCoordinate);
    Pixel originalColor = filledImage[startCoordinate.x][startCoordinate.y];
    while (!isEmpty(queue))
    {
        currentCoordinate = dequeue(&queue);
        filledImage[currentCoordinate.x][currentCoordinate.y] = newColor;
        if (currentCoordinate.x > 0 && filledImage[currentCoordinate.x - 1][currentCoordinate.y] == originalColor)
        {
            nextCoordinate.x = currentCoordinate.x - 1;
            nextCoordinate.y = currentCoordinate.y;
            enqueue(&queue, nextCoordinate);
        }
        if (currentCoordinate.y > 0 && filledImage[currentCoordinate.x][currentCoordinate.y - 1] == originalColor)
        {
            nextCoordinate.x = currentCoordinate.x;
            nextCoordinate.y = currentCoordinate.y - 1;
            enqueue(&queue, nextCoordinate);
        }
        if (currentCoordinate.x < d.h * d.r - 1 && filledImage[currentCoordinate.x + 1][currentCoordinate.y] == originalColor)
        {
            nextCoordinate.x = currentCoordinate.x + 1;
            nextCoordinate.y = currentCoordinate.y;
            enqueue(&queue, nextCoordinate);
        }
        if (currentCoordinate.y < d.lg * d.r - 1 && filledImage[currentCoordinate.x][currentCoordinate.y + 1] == originalColor)
        {
            nextCoordinate.x = currentCoordinate.x;
            nextCoordinate.y = currentCoordinate.y + 1;
            enqueue(&queue, nextCoordinate);
        }
    }
}

//-----------------------------------sort an image----------------------------------
// find the min of a line
int findMinInRow(Image image, Dimensions d, int startRow, int startCol)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    Pixel min = image[startRow][startCol];
    int i, j, minIndex = startRow;
    for (i = startRow; i < numRows; i++)
    {
        if (i == startRow)
        {
            for (j = startCol; j < numCols; j++)
                if (min > image[i][j])
                {
                    minIndex = i;
                    min = image[i][j];
                }
        }
        else
        {
            for (j = 0; j < numCols; j++)
                if (min > image[i][j])
                {
                    minIndex = i;
                    min = image[i][j];
                }
        }
    }
    return minIndex;
}

// find the min of a column
int findMinInColumn(Image image, Dimensions d, int startRow, int startCol)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    Pixel min = image[startRow][startCol];
    int i, j, minIndex = startCol;
    for (i = startRow; i < numRows; i++)
    {
        if (i == startRow)
        {
            for (j = startCol; j < numCols; j++)
                if (min > image[i][j])
                {
                    minIndex = j;
                    min = image[i][j];
                }
        }
        else
        {
            for (j = 0; j < numCols; j++)
                if (min > image[i][j])
                {
                    minIndex = j;
                    min = image[i][j];
                }
        }
    }
    return minIndex;
}

// swap two pixels
void swapPixels(Image image, int row1, int col1, int row2, int col2)
{
    Pixel temp;
    temp = image[row1][col1];
    image[row1][col1] = image[row2][col2];
    image[row2][col2] = temp;
}

// sorting algorithm
void sortImage(Image image, Dimensions d)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    int i, j;
    for (i = 0; i < numRows; i++)
        for (j = 0; j < numCols; j++)
            swapPixels(image, i, j, findMinInRow(image, d, i, j), findMinInColumn(image, d, i, j));
}

//------------------------------------dominant color-------------------------------
// know the number of distinct colors in an image
int countDistinctColors(Image image, Dimensions d)
{
    Image tempImage = createImage(d);
    copyImage(tempImage, image, d);
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    sortImage(tempImage, d);
    int distinctColorCount = 1, i, j;
    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < numCols - 1; j++)
            if (tempImage[i][j] != tempImage[i][j + 1])
                distinctColorCount++;
        if (i != numRows - 1 && tempImage[i][numCols - 1] != tempImage[i + 1][0])
            distinctColorCount++;
    }
    return distinctColorCount;
}

// get the colors of an image
PixelPointer getColors(Image image, Dimensions d)
{
    int numRows = d.h * d.r, numCols = d.lg * d.r;
    int distinctColorCount = countDistinctColors(image, d);
    PixelPointer colors = (PixelPointer)malloc(sizeof(Pixel) * distinctColorCount);
    Image tempImage = createImage(d);
    copyImage(tempImage, image, d);
    sortImage(tempImage, d);
    int colorIndex = 1, i, j;
    colors[0] = tempImage[0][0];
    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < numCols - 1; j++)
            if (tempImage[i][j] != tempImage[i][j + 1])
            {
                colors[colorIndex] = tempImage[i][j + 1];
                colorIndex++;
            }
        if (i != numRows - 1 && tempImage[i][numCols - 1] != tempImage[i + 1][0])
        {
            colors[colorIndex] = tempImage[i + 1][0];
            colorIndex++;
        }
    }
    return colors;
}

// frequency of a color in an image
float getColorFrequency(Image image, Dimensions d, Pixel color)
{
    int colorCount = 0;
    float frequency;
    for (int i = 0; i < d.r * d.h; i++)
    {
        for (int j = 0; j < d.r * d.lg; j++)
            if (image[i][j] == color)
                colorCount++;
    }
    frequency = float(colorCount * 100) / float(d.lg * d.h * d.r * d.r);
    return frequency;
}

// display colors of an image and their frequencies
void displayColorFrequencies(Image image, Dimensions d)
{
    PixelPointer colors = getColors(image, d);
    float maxFrequency = getColorFrequency(image, d, *(colors));
    int dominantColorIndex = 0;
    int distinctColorCount = countDistinctColors(image, d);
    for (int i = 0; i < distinctColorCount; i++)
    {
        printf("color%d : %u----------->%.2f %%\n\n", i, *(colors + i), getColorFrequency(image, d, *(colors + i)));
        if (maxFrequency < getColorFrequency(image, d, *(colors + i)))
        {
            maxFrequency = getColorFrequency(image, d, *(colors + i));
            dominantColorIndex = i;
        }
    }
    printf("\n\nThe dominant color is :%4u", *(colors + dominantColorIndex));
}
void applyNegativeFilter(Image negativeImage /*destination*/, Image originalImage /*source*/, Dimensions d)
{
    for (int i = 0; i < d.r * d.h; i++)
        for (int j = 0; j < d.r * d.lg; j++)
        {
            if (originalImage[i][j] < 128)
                negativeImage[i][j] = 0;
            else
                negativeImage[i][j] = 1;
        }
}
