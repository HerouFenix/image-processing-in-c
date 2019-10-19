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
    int height, width;
    unsigned char *bin_array;
} BinaryImage;

/********************************************/ /**
 *  FUNCTION DECLARATIONS
 ***********************************************/
///Function used to save a Binary image to a file
int save_to_file(char *file_name, BinaryImage *image);

/********************************************/ /**
 * Function used to load a Binary image
 *
 * @param file_name The File's path containing a Binary Image (PBM - P4)
 ***********************************************/
BinaryImage* load_file(char *file_name);

///Function used to acess a specific pixel within a Binary Image
unsigned char access_pixel(BinaryImage image, int line, int col);

///Function used to acess a subsection of image
BinaryImage get_subsection(BinaryImage image, int *pos_start, int *pos_end);

/********************************************/ /**
 *  FUNCTION DEFINITIONS
 ***********************************************/
int save_to_file(char *file_name, BinaryImage *image){
    FILE *fp;

    fp = fopen(file_name, "wb");
    if (!fp)
    {
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        return 1;
    }

    fprintf(fp, "P4\n");
    fprintf(fp, "%d %d\n", image->width, image->height);

    fwrite(image->bin_array, image->width, image->height, fp);

    return 0;
}

BinaryImage* load_file(char *file_name){
    FILE *fp;
    int comments;
    char img_type[8];
    BinaryImage* image;

    fp = fopen(file_name, "rb");
    if (!fp){
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if (!fgets(img_type, sizeof(img_type), (FILE*) fp) && (img_type[0] != 'P')){
        fprintf(stderr, "An error has ocurred trying to read the type of '%s', it may be malformatted or curropted\n", file_name);
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp); //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    image = (BinaryImage *)malloc(sizeof(BinaryImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "The PGM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    image->bin_array = (unsigned char *) malloc(image->width*image->height);

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    fread(image->bin_array, image->width, image->height, fp);

   /* int error;
    if ((error = fread(image->bin_array, image->width, image->height, fp)) != image->height)
    { //Read all remaining data from the image (Remember each grayscale pixel is just a value from 0-255)
        fprintf(stderr, "At least one line %d couldn't be read from the image\n", error);
        exit(1);
    }*/

    fclose(fp);
    return image;
}

int main()
{
	BinaryImage *image = load_file("../marbles.pbm");
    return save_to_file("marbles.pbm", image);
}