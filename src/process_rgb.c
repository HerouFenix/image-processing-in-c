#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************/ /**
 *  STRUCTURE DECLARATIONS
 ***********************************************/
/// Structure used to represent RGB colours (pixels)
/// Unsigned Char since we only need to specify values from 0 to 255, which can be done with 1 byte (which corresponds to the size of an unsigned char)
typedef struct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Colour;

/// Structure used to save an RGB image
typedef struct
{
    int width, height;
    Colour *pixel_array;
} RGBImage;

/********************************************/ /**
 *  FUNCTION DECLARATIONS
 ***********************************************/
///Function used to save an RGB image to a file
int save_to_file(char *file_name, RGBImage *image);

///Function used to load an RGB image
RGBImage *load_file(char *file_name);

///Function used to acess a specific pixel within an RGB Image
Colour access_pixel(RGBImage image, int line, int col);

///Function used to acess a subsection of image
RGBImage get_subsection(RGBImage image, int *pos_start, int *pos_end);

/********************************************/ /**
 *  FUNCTION DEFINITION
 ***********************************************/
int save_to_file(char *file_name, RGBImage *image)
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

    img_type = "P6";
    height = image->height, width = image->width, colour_range = 255;

    //Write the PPM's headers
    fprintf(fp, "%s\n", img_type);
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", colour_range);

    //Write the image's pixels
    fwrite(image->pixel_array, 3 * width, height, fp);

    fclose(fp);
    return 0;
};

//TO DO: VERIFY IF COMMENTS CAN BE PLACED ON ANY PLACE IN THE DOCUMENT OR WHATS THE DEAL WITH THAT!
RGBImage *load_file(char *file_name)
{
    char image_type[8];
    int colour_range, comments;
    FILE *fp;
    RGBImage *image;

    fp = fopen(file_name, "rb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if ((!fgets(image_type, sizeof(image_type), fp)) && (image_type[0] != 'P' && image_type[1] != '6'))
    { //Get PPM Type (Goes from P0-P6)
        fprintf(stderr, "The PPM image file seems to be malformed\n");
        exit(1);
    }

    image = (RGBImage *)malloc(sizeof(RGBImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "The PPM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    if ((fscanf(fp, "%d", &colour_range) != 1) && (colour_range != 255))
    { //Get colour range (should be 255)
        fprintf(stderr, "The PPM image seems to have an invalid rgb range - Should be 255\n");
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp);   //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    image->pixel_array = (Colour *)malloc(sizeof(Colour) * image->width * image->height); //Allocate memory for image's pixels

    if (fread(image->pixel_array, 3 * image->width, image->height, fp) != image->height)
    { //Read all remaining data from the image (we multiply the width by 3 because each pixel holds 3 chars - r,g,b)
        fprintf(stderr, "At least one line couldn't be read from the image\n");
        exit(1);
    }

    fclose(fp);
    return image;
};

int main()
{
    RGBImage *image = load_file("../lena.ppm");
    save_to_file("lena.ppm", image);
    return 0;
}