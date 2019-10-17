#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************/ /**
 *  STRUCTURE DECLARATIONS
 ***********************************************/
/// Structure used to represent Binary images
/**
 *  The way we chose to represent our bits was through the use of an array of chars (1 byte structures). 
 *  Instead of thinking of the image as a bidimensional array, we simply used a linear array noting that the image position can be given by the formula:
 *  position = line*noOfColumns+column
*/
typedef struct
{
    unsigned char *BinImage;
} BinaryImage;

/********************************************/ /**
 *  FUNCTION DECLARATIONS
 ***********************************************/
///Function used to save a Binary image to a file
int save_to_file(char *file_name, BinaryImage image);

///Function used to load a BInary image
BinaryImage load_file(char *file_name);

///Function used to acess a specific pixel within a Binary Image
unsigned char access_pixel(BinaryImage image, int line, int col);

///Function used to acess a subsection of image
BinaryImage get_subsection(BinaryImage image, int *pos_start, int *pos_end);

int main()
{
    return 0;
}