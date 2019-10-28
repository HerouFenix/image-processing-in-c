#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process_rgb.h"
#include "process_grayscale.h"
#include "image_processing.h"

#define NUMBER_OF_STRING 10
#define MAX_NUMBER_OF_OPERATIONS 100

RGBImage *RGB_IMAGE;
RGBImage *RGB_OG;
RGBImage *RGB_BACKUP;
char RGB_OPERATIONS[NUMBER_OF_STRING][MAX_NUMBER_OF_OPERATIONS];

GrayscaleImage *GRAY_IMAGE;
GrayscaleImage *GRAY_OG;
GrayscaleImage *GRAY_BACKUP;
char GRAYSCALE_OPERATIONS[NUMBER_OF_STRING][MAX_NUMBER_OF_OPERATIONS];

GrayscaleImage *RGB_TO_GRAY_IMAGE;
GrayscaleImage **RGB_TO_GRAY_IMAGES;

int main()
{
    char option;

    while (1)
    {
        //Clear the screen
        system("clear");

        //Clears the keyboard buffer
        fflush(stdin);

        puts("\nPick an Operation"
            "\n[L]oad Image"
             "\n[S]ave Image"
             "\n[R]eset Image"
             "\n[B]ackup Image"
             "\n[T]urn Image"
             "\n[C]ut Image"
             "\n[F]ilter Image"
             "\n[I]ntensify Image"
             "\n[A]dapt Image"
             "\n[H]istory of changes to Image"
             "\n.........."
             "\n[x]Exit");
        option = getchar();

        switch (option)
        {
        case 'L': //Load Image
            //Clears the keyboard buffer
            fflush(stdin);

            puts("\nWhat type of image do you want to load?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 "\n.........."
                 "\n[C]ancel");
            option = getchar();

            if(option=='R'){
                fflush(stdin);
                puts("\nType down your image's path: ");
                char file_path[256];
                fgets(file_path, sizeof(file_path), stdin);

                RGB_IMAGE = load_rgb_file(file_path);
                RGB_OG = copy_rgb_image(RGB_IMAGE);

            }else if(option=='G'){
                fflush(stdin);
                puts("\nType down your image's path: ");
                char file_path[256];
                fgets(file_path, sizeof(file_path), stdin);

                GRAY_IMAGE = load_grayscale_file(file_path);
                GRAY_OG = copy_grayscale_image(GRAY_IMAGE);

            }else if(option=='C'){
                break;
            }
            
        case 'S': //Save Image
            //TO DO CODE
            break;
        case 'R': //Reset current image
            //TO DO CODE
            break;
        case 'B':
            //TO DO CODE
            break;
        case 'T':
            //TO DO CODE
            break;
        case 'C':
            //TO DO CODE
            break;
        case 'F':
            //TO DO CODE
            break;
        case 'I':
            //TO DO CODE
            break;
        case 'A':
            //TO DO CODE
            break;
        case 'H':
            //TO DO CODE
            break;
        //...
        case 'x':
            break;
        default:
            puts("\nYou've entered an invalid command!\nDoing nothing...");
        }
        return 0;
    }
}