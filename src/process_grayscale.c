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
/**
 *  The way we chose to represent our pixels was through the use of an array of chars (1 byte structures). 
 *  Instead of thinking of the image as a bidimensional array, we simply used a linear array noting that the image position can be given by the formula:
 *  position = line*noOfColumns+column
*/
typedef struct
{
    int width, height;
    Grayscale *pixel_array;
} GrayscaleImage;

/********************************************/ /**
 *  FUNCTION DECLARATIONS
 ***********************************************/
/********************************************/ /**
 * Function used to save a Grayscale image to a file
 *
 * @param file_name The File's path on which we'll be saving the image
 * @param image The Grayscale Image we want to save
 ***********************************************/
int save_to_file(char *file_name, GrayscaleImage *image);

/********************************************/ /**
 * Function used to load a Grayscale image
 *
 * @param file_name The File's path containing a Grayscale Image (PGM - P5)
 ***********************************************/
GrayscaleImage *load_file(char *file_name);

/********************************************/ /**
 * Function used to acess a specific pixel within a Grayscale Image
 *
 * @param image The Grayscale image we want to subsect
 * @param line The pixel's line (y position)
 * @param col The pixel's col (x position)
 ***********************************************/
Grayscale access_pixel(GrayscaleImage *image, int line, int col);

/********************************************/ /**
 * Function used to acess a subsection of image
 *
 * @param image The Grayscale image we want to subsect
 * @param pos_start An array containing the subsection's starting starting (left-top corner) x and y coordinates
 * @param pos_end An array containing the subsection's ending (bottom-right corner) x and y coordinates
 ***********************************************/
GrayscaleImage get_subsection(GrayscaleImage *image, int pos_start[2], int pos_end[2]);

/********************************************/ /**
 *  FUNCTION DEFINITIONS
 ***********************************************/
int save_to_file(char *file_name, GrayscaleImage *image)
{
    FILE *fp;
    int width, height, colour_range;
    char *img_type;

    fp = fopen(file_name, "wb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        return 1;
    }

    img_type = "P5";
    height = image->height, width = image->width, colour_range = 255;

    //Write the PPM's headers
    fprintf(fp, "%s\n", img_type);
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", colour_range);

    //Write the image's pixels
    fwrite(image->pixel_array, width, height, fp);

    fclose(fp);
    return 0;
};

//TO DO: VERIFY IF COMMENTS CAN BE PLACED ON ANY PLACE IN THE DOCUMENT OR WHATS THE DEAL WITH THAT!
GrayscaleImage *load_file(char *file_name)
{
    char image_type[8];
    int comments, colour_range = 0;
    FILE *fp;
    GrayscaleImage *image;

    fp = fopen(file_name, "rb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if ((!fgets(image_type, sizeof(image_type), fp)) && (image_type[0] != 'P' && image_type[1] != '5'))
    { //Get PPM Type (Goes from P0-P6)
        fprintf(stderr, "The PGM image file seems to be malformed or is incorrect - Should be P5\n");
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp); //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    image = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "The PGM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    if ((fscanf(fp, "%d", &colour_range) != 1) && (colour_range != 255))
    { //Get colour range (should be 255)
        fprintf(stderr, "The PGM image seems to have an invalid grayscale range - Should be 255\n");
        exit(1);
    }

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    image->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * image->width * image->height); //Allocate memory for image's pixels

    if (fread(image->pixel_array, image->width, image->height, fp) != image->height)
    { //Read all remaining data from the image (Remember each grayscale pixel is just a value from 0-255)
        fprintf(stderr, "At least one line couldn't be read from the image\n");
        exit(1);
    }

    fclose(fp);
    return image;
};

Grayscale get_pixel(GrayscaleImage *image, int line, int col)
{
    int index = line * image->width + col; //The index of the pixel is given by the formula: pixel_line * no_columns + pixel_column
    return image->pixel_array[index];
}

int main()
{
    GrayscaleImage *image = load_file("../galaxy.ascii.pgm");

    Grayscale pixel = get_pixel(image, image->width / 2, image->height / 2);
    printf("Gray%d\n", pixel.Gray);

    save_to_file("galaxy.pgm", image);
    return 0;
}
