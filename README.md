# Image Processing Examples
This repo has/will have a few image processing codes for different uses that I wrote in university when I took the course.

##Example 1: Sobel Filter (Operator) Implementation Using C For PGM P5
This code is an implementation of the Sobel Filter in C. When a Sobel Filter is applied to an image, it will output a new image which is black and white; whites are outlines of the image and blacks are the rest. This is used for edge detection./
Program only works on type P5 PGM images. When you run the program, it will ask you the name of the program (user must enter .pgm extention as well). Program will write the original image, image after gradiant x calculations, image after gradiant y calculations and image after Sobel Filter is completely applied to the current directory with the names of Img.pgm, xEdgeDetectedImg.pgm, yEdgeDetectedImg.pgm and EdgeDetectedImg.pgm respectively.
