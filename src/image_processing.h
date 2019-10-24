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
 * @param rgb_image The Grayscale image we want to subsect
 ***********************************************/
GrayscaleImage** convert_rgb_to_three_grayscale(RGBImage *rgb_image);
