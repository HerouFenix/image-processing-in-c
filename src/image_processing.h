/********************************************//**
 *  FUNCTION DECLARATIONS
 ***********************************************/
/********************************************/ /**
 * Function used to create a grayscale image out of an rgb one by computing all of its r, g and b parameters
 *
 * @param rgb_image The RGB Image we want to convert
 ***********************************************/
GrayscaleImage* convert_rgb_to_grayscale(RGBImage *rgb_image);

/********************************************/ /**
 * Function used to create a grayscale image out of a single parameter of an RGB one (R, G or B). Returns an array of GrayScaleImages where it's positions
 * correspond to the Red Grayscale, Green Grayscale and Blue Grayscale (in this order)
 *
 * @param rgb_image The RGB image we want to convert into 3 Grayscales
 ***********************************************/
GrayscaleImage** convert_rgb_to_three_grayscale(RGBImage *rgb_image);

/********************************************/ /**
 * Function used to apply a filter kernel on a given RGB image, returning 0 if it succeeded or 1 if it failed
 *
 * @param image The RGB image we want to apply the filter to
 * @param kernel The Kernel array (as a linear array representing a 2d value matrix)
 * @param image The Kernel's dimensions (filter_dimensions[0] - # of rows ; filter_dimension[1] - # of cols)

 ***********************************************/
int apply_rgb_filter(RGBImage *image, double *kernel, int filter_dimension[2]);

/********************************************/ /**
 * Function used to apply a filter kernel on a given Grayscale image, returning 0 if it succeeded or 1 if it failed
 *
 * @param image The Grayscale image we want to apply the filter to
 * @param kernel The Kernel array (as a linear array representing a 2d value matrix)
 * @param image The Kernel's dimensions (filter_dimensions[0] - # of rows ; filter_dimension[1] - # of cols)

 ***********************************************/
int apply_grayscale_filter(GrayscaleImage *image, double *kernel, int filter_dimension[2]);