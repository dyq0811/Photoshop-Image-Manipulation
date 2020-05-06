# Photoshop Image Manipulation
An image manipulation program, in the vein of Photoshop.

## Authors
[Yingqi Ding](https://github.com/dyq0811) & Vivian Looi

##
The operations our program is able to recognize and perform (the bolded words are the operation names to be entered at the command line by the user):
1. exposure - change exposure of an image
2. α-blend - blend two images by a given ratio
3. zoom in - zoom in to an image
4. zoom out - zoom out of an image
5. pointilism - pointilism technique
6. swirl - swirl the image
7. blur - blur the image (smooth and soften its edges) by a specified amount

For example, at the command prompt, the user might type:

./project trees.ppm trees-exp-half.ppm exposure 0.5

to take as input a PPM file named trees.ppm and create an output file named trees-exp-half.ppm which contains a new version of the trees.ppm with an increased exposure by a factor of 0.5. Or, if the desire is to blend trees.ppm into buidling.ppm with a blending ratio of 0.5, then the user could type:

./project trees.ppm trees-building-blended.ppm blend building.ppm 0.5

to generate the output file named trees-buidling-blended.ppm which is a blend of both images.

## Implementation Process
For the read_ppm function, it first assert if the file pointer given as the parameter is not null, then ignores the tag and comments, then assert that the number of rows and columns in the image aren't 0, and that the colors is 255. The function then creates an Image structure and allocate a pointer pointing to the dynamic memory of the size of Image. cols and rows in the structure are assigned to the number of columns and rows of the image specified at the top of the file respectively. data is allocated with a pointer pointing to the dynamic memory of the size of a pixel times the dimensions of the image. At last, binary data encoding pixels are read into data. The file pointer is closed, and the Image created is returned to the main function. 


In the imageManip.c file, we wrote three helper methods. The new_im method returns a new Image structure with a pointer pointing to a dynamic memory of the size of an Image structure allocated, and cols, rows, and data correctly assigned. The sq method uses the pow() function in the math library to square the given double. The color_constraint method returns a valid color code by returning the lower bound if the provided color code is smaller than the lower bound, and returning the upper bound if the provided color code is larger than the upper bound.


For the exposure function, every color code is multiplied by 2^EV. This value is passed into the color_constraint method to ensure that color codes are valid.


For the blend function, a new Image structure is first created which has the greater number of columns and greater number of rows out of the two input files. Then, the value of each pixel in the new image is calculated using the given formula. For pixels which do not exist in any one of the input image, the value will be equal to the value in the input image which the pixels exist. Pixels out of bound for both input images will be rendered as black.


For the zoom_in function, a new Image structure is first created which has 2 times the number of rows and columns of the input image. Even-numbered rows in the new image are first filled by duplicating each pixel in the original image, then the odd-numbered rows in the new image are filled by duplicating the previous row.


For the zoom_out function, a new Image structure is first created which has the number of rows and columns of the input image divided by 2 (integer division). For each pixel in the new image, its value is determined by averaging the pixel values in the 2x2 square with row number *2 and column number * 2 as the left upper corner.


For the pointilism function, after picking a random central pixel and radius, pixels within the radius from the central pixel will be set to the same value as the central pixel. This operation is repeated for the total number of pixels * 0.03 times.


For the swirl function, a new Image structure is first created which has the smae number of rows and columns as the input image. Then the value of each pixel is set as the value of the pixel of coordinates calculated using the given formula. If the coordinates are out-of-bound, the pixel is rendered as black.

For the blur function, a 2D array is created and initialized with values calculated from the equation given as the Gaussian "filter" matrix. And we apply this filter pixel by pixel for pixels that are elligible to be the center of a circle whose radius is not out of boundary.
