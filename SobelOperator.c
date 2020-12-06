#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct PGMstructure{ //PGM structure
    int maxVal; //maximum pixel value in the file
    int width; //width of the image
    int height; //height of the image
    unsigned int** data; //matrix that holds gray values
};

typedef struct PGMstructure PGMImage;

void save(int** image, int width, int height, char* save_location){ //function that writes every value in the integer matrix 'image' as chars to a file
    int i,j, value; //index variables and the value variable that contains pixel value to be written
    FILE* out_img; //file that will contain output image
    printf("\nWriting the pixels to file.");

    out_img = fopen(save_location, "wb"); //file to be written is opened as binary

    fprintf(out_img, "P5\n"); //writing the metadata
    fprintf(out_img, "%d %d\n", width, height);
    fprintf(out_img, "255\n");

    for (j = 0; j < height; j++) { //each pixel value is written to the file as unsigned chars
        for (i = 0; i < width; i++) {
            value = (unsigned char) (image[j][i]); //integer value is being converted to unsigned char
            fwrite(&value, sizeof(unsigned char), 1, out_img); //writing this unsigned char value to the file
        }
    }
    printf("\nWriting is done.");

    fclose(out_img); //file is closed
}

void applySobel(PGMImage *img){ //function that applies Sobel filter
    printf("\nSobel filter is applying to the image.");
    int** filtered_img_x; // matrix outcome of the image using sobel filter in x direction
    int** filtered_img_y; // matrix outcome of the image using sobel filter in y direction
    int** filtered_img_final; // final outcome matrix
    int i, j, max_x, min_x, min_y, max_y, min, max; // index variables and min-max pixel gray values for each direction to normalize it later
    int center_pixel_value; // will keep the result of the pixel's values after each step in sobel filter

    filtered_img_x = (int**) calloc ((*img).height + 1, sizeof(int*)); // for each matrix necessary memory allocation is done dynamically
    filtered_img_y = (int**) calloc ((*img).height + 1, sizeof(int*));
    filtered_img_final = (int**) calloc ((*img).height + 1, sizeof(int*));
    for (i = 0; i<(*img).height + 1; i++){
        filtered_img_x[i] = (int*) calloc ((*img).width + 1, sizeof(int));
        filtered_img_y[i] = (int*) calloc ((*img).width + 1, sizeof(int));
        filtered_img_final[i] = (int*) calloc ((*img).width + 1, sizeof(int));
    }
    //instead of using padding, steps for edges and corners are done separately from central pixels to eleminate the need of padding, therefore saving memory space and runtime

    center_pixel_value = 0; //x value of the pixel at the top left
    center_pixel_value += (*img).data[0][1] * 2;
    center_pixel_value += (*img).data[1][1];
    filtered_img_x[0][0] = center_pixel_value;

    center_pixel_value = 0; //y value of the pixel at the top left
    center_pixel_value +=(*img).data[1][0] * 2;
    center_pixel_value += (*img).data[1][1];
    filtered_img_y[0][0] = center_pixel_value;

    center_pixel_value = sqrt(filtered_img_x[0][0]*filtered_img_x[0][0] + filtered_img_y[0][0]*filtered_img_y[0][0]); // total value
    filtered_img_final[0][0] = center_pixel_value;

    i = (*img).width - 1;
    center_pixel_value = 0; //x value of the pixel at the top right
    center_pixel_value -= (*img).data[0][i-1] * 2;
    center_pixel_value -= (*img).data[1][i-1];
    filtered_img_x[0][i] = center_pixel_value;

    center_pixel_value = 0; //y value of the pixel at the top right
    center_pixel_value +=(*img).data[1][i] * 2;
    center_pixel_value += (*img).data[1][i-1];
    filtered_img_y[0][i] = center_pixel_value;

    center_pixel_value = sqrt(filtered_img_x[0][i]*filtered_img_x[0][i] + filtered_img_y[0][i]*filtered_img_y[0][i]); // total value
    filtered_img_final[0][i] = center_pixel_value;

    j = (*img).height - 1;
    center_pixel_value = 0; //x value of the pixel at the bottom left
    center_pixel_value += (*img).data[j][1] * 2;
    center_pixel_value += (*img).data[j-1][1];
    filtered_img_x[j][0] = center_pixel_value;

    center_pixel_value = 0; //y value of the pixel at the bottom left
    center_pixel_value -=(*img).data[j-1][0] * 2;
    center_pixel_value -= (*img).data[j-1][1];
    filtered_img_y[j][0] = center_pixel_value;

    center_pixel_value = sqrt(filtered_img_x[j][0]*filtered_img_x[j][0] + filtered_img_y[j][0]*filtered_img_y[j][0]); // total value
    filtered_img_final[j][0] = center_pixel_value;

    center_pixel_value = 0; //x value of the pixel at the bottom right
    center_pixel_value -= (*img).data[j][i-1] * 2;
    center_pixel_value -= (*img).data[j-1][i-1];
    filtered_img_x[j][i] = center_pixel_value;

    center_pixel_value = 0; //y value of the pixel at the bottom right
    center_pixel_value -=(*img).data[j-1][i] * 2;
    center_pixel_value -= (*img).data[j-1][i-1];
    filtered_img_y[j][i] = center_pixel_value;

    center_pixel_value = sqrt(filtered_img_x[j][i]*filtered_img_x[j][i] + filtered_img_y[j][i]*filtered_img_y[j][i]); // total value
    filtered_img_final[j][i] = center_pixel_value;

    for(i = 1; i < (*img).width - 1; i++){ //finds values of the bottom and top edges after the filter
        center_pixel_value = 0; //x values for the top edge
        center_pixel_value -= (*img).data[0][i-1] * 2;
        center_pixel_value += (*img).data[0][i+1] * 2;
        center_pixel_value -= (*img).data[1][i-1];
        center_pixel_value += (*img).data[1][i+1];
        filtered_img_x[0][i] = center_pixel_value;

        center_pixel_value = 0; //x values for the bottom edge
        center_pixel_value -= (*img).data[j][i-1] * 2;
        center_pixel_value += (*img).data[j][i+1] * 2;
        center_pixel_value -= (*img).data[j-1][i-1];
        center_pixel_value += (*img).data[j-1][i+1];
        filtered_img_x[j][i] = center_pixel_value;


        center_pixel_value = 0; //y values for the top edge
        center_pixel_value += (*img).data[1][i-1];
        center_pixel_value += (*img).data[1][i] * 2;
        center_pixel_value += (*img).data[1][i+1];
        filtered_img_y[0][i] = center_pixel_value;

        center_pixel_value = 0; //y values for the bottom edge
        center_pixel_value -= (*img).data[j-1][i-1];
        center_pixel_value -= (*img).data[j-1][i] * 2;
        center_pixel_value -= (*img).data[j-1][i+1];
        filtered_img_y[j][i] = center_pixel_value;

        center_pixel_value = sqrt(filtered_img_x[0][i]*filtered_img_x[0][i] + filtered_img_y[0][i]*filtered_img_y[0][i]); // total value for the top edge
        filtered_img_final[0][i] = center_pixel_value;

        center_pixel_value = sqrt(filtered_img_x[j][i]*filtered_img_x[j][i] + filtered_img_y[j][i]*filtered_img_y[j][i]); // total value for the bottom edge
        filtered_img_final[j][i] = center_pixel_value;

    }

    i = (*img).width - 1;
    for (j = 1; j < (*img).height - 1; j++){ //finds values of the left and right edges after the filter
        center_pixel_value = 0; //x values for the left edge
        center_pixel_value += (*img).data[j-1][1];
        center_pixel_value += (*img).data[j][1] * 2;
        center_pixel_value += (*img).data[j+1][1];
        filtered_img_x[j][0] = center_pixel_value;

        center_pixel_value = 0; //x values for the right edge
        center_pixel_value -= (*img).data[j-1][i-1];
        center_pixel_value -= (*img).data[j][i-1] * 2;
        center_pixel_value -= (*img).data[j+1][i-1];
        filtered_img_x[j][i] = center_pixel_value;


        center_pixel_value = 0; //y values for the left edge
        center_pixel_value -= (*img).data[j-1][0] * 2;
        center_pixel_value -= (*img).data[j-1][1];
        center_pixel_value += (*img).data[j+1][0] * 2;
        center_pixel_value += (*img).data[j+1][1];
        filtered_img_y[j][0] = center_pixel_value;

        center_pixel_value = 0; //y values for the right edge
        center_pixel_value -= (*img).data[j-1][i] * 2;
        center_pixel_value -= (*img).data[j-1][i-1];
        center_pixel_value += (*img).data[j+1][i] * 2;
        center_pixel_value += (*img).data[j+1][i-1];
        filtered_img_y[j][i] = center_pixel_value;

        center_pixel_value = sqrt(filtered_img_x[j][0]*filtered_img_x[j][0] + filtered_img_y[j][0]*filtered_img_y[j][0]); // total value for the left edge
        filtered_img_final[j][0] = center_pixel_value;

        center_pixel_value = sqrt(filtered_img_x[j][i]*filtered_img_x[j][i] + filtered_img_y[j][i]*filtered_img_y[j][i]); // total value for the right edge
        filtered_img_final[j][i] = center_pixel_value;
    }

    for(i = 1; i < (*img).height - 1; i++){ //values of pixels that are not at edges
        for (j = 1; j < (*img).width - 1; j++){
            center_pixel_value = 0;
            center_pixel_value -= (*img).data[i-1][j-1];
            center_pixel_value -= (*img).data[i][j-1] * 2;
            center_pixel_value -= (*img).data[i+1][j-1];
            center_pixel_value += (*img).data[i-1][j+1];
            center_pixel_value += (*img).data[i][j+1] * 2;
            center_pixel_value += (*img).data[i+1][j+1];
            filtered_img_x[i][j] = center_pixel_value;

            center_pixel_value = 0;
            center_pixel_value -= (*img).data[i-1][j-1];
            center_pixel_value -= (*img).data[i-1][j] * 2;
            center_pixel_value -= (*img).data[i-1][j+1];
            center_pixel_value += (*img).data[i+1][j-1];
            center_pixel_value += (*img).data[i+1][j] * 2;
            center_pixel_value += (*img).data[i+1][j+1];
            filtered_img_y[i][j] = center_pixel_value;

            center_pixel_value = sqrt(filtered_img_x[i][j]*filtered_img_x[i][j] + filtered_img_y[i][j]*filtered_img_y[i][j]); // total value
            filtered_img_final[i][j] = center_pixel_value;
        }
    }

    min_x = 0;
    max_x = 0;
    min_y = 0;
    max_y = 0;
    min = 0;
    max = 0;

    for(j = 0; j < (*img).height; j++){ // finds maximum and minimum values
        for(i = 0; i < (*img).width; i++){
            if (filtered_img_x[j][i] < min_x)
                min_x = filtered_img_x[j][i];
            else if(filtered_img_x[j][i] > max_x)
                max_x = filtered_img_x[j][i];
            if (filtered_img_y[j][i] < min_y)
                min_y = filtered_img_y[j][i];
            else if(filtered_img_y[j][i] > max_y)
                max_y = filtered_img_y[j][i];
            if (filtered_img_final[j][i] < min)
                min = filtered_img_final[j][i];
            else if(filtered_img_final[j][i] > max)
                max = filtered_img_final[j][i];
        }
    }

    for(j = 0; j < (*img).height; j++){ //normalization of the pixels for 0-255 value range
        for(i = 0; i < (*img).width; i++){ //to prevent data loss from float values, multiplication and division operations are done using a float value, result is stored in an integer
            filtered_img_x[j][i] = 255.0f * (filtered_img_x[j][i] - min_x) / (max_x - min_x);
            filtered_img_y[j][i] = 255.0f * (filtered_img_y[j][i] - min_y) / (max_y - min_y);
            filtered_img_final[j][i] = 255.0f * (filtered_img_final[j][i] - min) / (max - min);
        }
    }


    save((*img).data, (*img).width, (*img).height, "Img.pgm"); //writes original image, after sobel filter is applied at x and y direction, and the final edge detected image
    save(filtered_img_x, (*img).width, (*img).height, "xEdgeDetectedImg.pgm");
    save(filtered_img_y, (*img).width, (*img).height, "yEdgeDetectedImg.pgm");
    save(filtered_img_final, (*img).width, (*img).height, "edgeDetectedImg.pgm");

}

void PGMAc(char file_name[], PGMImage *img){ //opens the file and saves the contents to img variable
    FILE* input_file; //file pointer of the input file
    char ch; //variable that will be used for each scanned character
    unsigned char buffer[100]; // buffer to be used by fread()
    int row, col, pgm_type, k; // index variables and the variable that keeps the type of the PGM file
    int ch_int; //variable that will be used for each scanned integer

    input_file = fopen(file_name, "rb"); // file is opened as read binary
    if (input_file == NULL){ //if file is not found, an error message will appear and program will close
        printf("\nFile can not be found");
        fclose(input_file);
        exit(0);
    }

    printf("\nOpening the file");

    //metadata is scanned before pixel values, because for pixel values scanning with buffer is ideal but if we were to scan the metadata in the same loop
    //with pixel values, extra checks and procedures are need to be written which would increase the processing load

    ch = getc(input_file); //checks if a file is a PGM file
    if(ch != 'P'){
        printf("\nThis file is not a PGM file");
        fclose(input_file);
        exit(0);
    }

    ch = getc(input_file); //checks if file is a P5 type PGM file
    pgm_type = ch - 48;
    if(pgm_type != 5){
        printf("\nPlease select a PGM file that is P5 type");
        fclose(input_file);
        exit(0);
    }

    while(getc(input_file) != '\n'){} //goes to the end of the line

    while (getc(input_file) == '#'){ //if there is a comment goes to the end of the line (comment is expected to be a single line comment, otherwise program will not work as intended)
        while(getc(input_file) != '\n'){}
    }

    fseek(input_file, -1, SEEK_CUR);

    //metadata is read and printed to the screed
    fscanf(input_file,"%d", &((*img).width));
    fscanf(input_file,"%d", &((*img).height));
    fscanf(input_file,"%d", &((*img).maxVal));

    printf("\n width  = %d",(*img).width);
    printf("\n height = %d",(*img).height);
    printf("\n maxVal = %d",(*img).maxVal);
    printf("\n");

    (*img).data = (unsigned int**) calloc((*img).height + 1, sizeof(unsigned int*)); //values that are scanned are to be kept in this dynamically allocated matrix
    for (row = 0; row < (*img).height; row++){
        (*img).data[row] = (unsigned int*) calloc((*img).width + 1, sizeof(unsigned int));
    }
    row = 0;
    col = 0;
    fseek(input_file, 1, SEEK_CUR);
    while(!feof(input_file)){ //scans 100 characters and saves them until cursor goes to the end of the file
        fread(buffer, sizeof(buffer), 1, input_file);
        k = 0;
        while (k < 100 && (*img).height > row){
            (*img).data[row][col] = buffer[k];
            col++;
            if(col >= (*img).width){ // if it's the end of the line, proceeds to the next line
                col = 0;
                row++;
            }
            k++;
        }
    }
}

int main(){
    char file_name[50];
    PGMImage img; //Structure variable that will keep all data of a given PGM file
    printf("\nEnter the name of the image to be edge detected (add .pgm extension as well).\n");
    scanf("%s", file_name);
    PGMAc(file_name, &img); //opens the pgm file and stores the data in img variable
    applySobel(&img); //passes img to the function that will apply Sobel filter and save the resulting image
    return 0;
}
