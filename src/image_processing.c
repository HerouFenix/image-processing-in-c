#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************//**
 *  STRUCTURE DECLARATIONS
 ***********************************************/
/// Structure used to represent RGB colours
/// Unsigned Char since we only need to specify values from 0 to 255, which can be done with 1 byte (which corresponds to the size of an unsigned char)
typedef struct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Colour;

/// Structure used to represent Grayscale values
/// Unsigned Char since we only need to specify values from 0 to 255, which can be done with 1 byte (which corresponds to the size of an unsigned char)
typedef struct
{
    unsigned char Gray;
} Grayscale;

/// Structure used to represent Binary images
/**
 *  The way we chose to represent our bits was through the use of an array of chars (1 byte structures). 
 *  Instead of thinking of the image as a bidimensional array, we simply used a linear array noting that the image position can be given by the formula:
 *  position = line*noOfColumns+noOfColumns
*/
typedef struct
{
    unsigned char *Image;
} BinaryImage;



/********************************************//**
 *  FUNCTION DECLARATIONS
 ***********************************************/
int save_to_file(char* file_name);
int load_file(char* file_name);

int main(){
    return 0;
}