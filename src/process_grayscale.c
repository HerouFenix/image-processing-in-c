#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************/ /**
 *  STRUCTURE DECLARATIONS
 ***********************************************/
/// Structure used to represent Grayscale values
/// Unsigned Char since we only need to specify values from 0 to 255, which can be done with 1 byte (which corresponds to the size of an unsigned char)
typedef struct
{
    unsigned char Gray;
} Grayscale;

/// Structure used to represent Grayscale images
typedef struct
{
    Grayscale *GrayImage;
} GrayscaleImage;

/********************************************/ /**
 *  FUNCTION DECLARATIONS
 ***********************************************/
///Function used to save a Grayscale image to a file
int save_to_file(char *file_name, GrayscaleImage image);

///Function used to load a Grayscale image
GrayscaleImage load_file(char *file_name);

///Function used to acess a specific pixel within a Grayscale Image
Grayscale access_pixel(GrayscaleImage image, int line, int col);

///Function used to acess a subsection of image
GrayscaleImage get_subsection(GrayscaleImage image, int *pos_start, int *pos_end);

int main()
{
    return 0;
}