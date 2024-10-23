#include "film.h"

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

#include <iostream>
#include <vector>

/**
 * @brief Film::Film
 */

Film::Film(size_t width_, size_t height_)
{
    // Initialize the width and height of the image
    width  = width_;
    height = height_;

    // Allocate memory for the image matrix
    data = new Vector3D*[height];
    for( size_t i=0; i<height; i++)
    {
        data[i] = new Vector3D[width];
    }

    // Set all values to zero
    clearData();
}

Film::~Film()
{
    // Resease the dynamically-allocated memory for the image data
    for( size_t i=0; i<height; i++)
    {
        delete [] data[i];
    }
    delete [] data;
}

size_t Film::getWidth() const
{
    return width;
}

size_t Film::getHeight() const
{
    return height;
}

Vector3D Film::getPixelValue(size_t w, size_t h) const
{
    return data[h][w];
}

void Film::setPixelValue(size_t w, size_t h, Vector3D &value)
{
    data[h][w] = value;
}

void Film::clearData()
{
    Vector3D zero;

    for(size_t h=0; h<height; h++)
    {
        for(size_t w=0; w<width; w++)
        {
            setPixelValue(w, h, zero);
        }
    }
}

int Film::save()
{
    return BitMap::save(data, width, height);
}


int Film::saveEXR()
{
    const char* fname = "output.exr";

    unsigned int N_COMPONENTS = 3;

    ImageBufferEXR buffer;
    buffer.data = data;
    buffer.width = width;
    buffer.height = height;

    const std::string filename(fname);

    float* myImage = (float*)malloc(sizeof(float) * width* height* N_COMPONENTS);    
  
    for (size_t i = 0; i < static_cast<size_t>(width); i++) {
        for (size_t j = 0; j < static_cast<size_t>(height); j++) {
            int idx = j * width + i;
            myImage[idx * 3 + 0] = data[height - j - 1][i].x;
            myImage[idx * 3 + 1] = data[height - j - 1][i].y;
            myImage[idx * 3 + 2] = data[height - j - 1][i].z;
        }
    }

    const char* err = "";
    int32_t ret = SaveEXR(
        myImage,
        buffer.width,
        buffer.height,
        N_COMPONENTS, // num components
        0, // save_as_fp32
        filename.c_str(),
        &err);

    free(myImage);

    bool saved_correctly = (err != NULL) && (err[0] == '\0');
    if (saved_correctly) {
        printf("EXR Stored Correctly :) \n"); 
        return 1;
    }
    else {
        std::cout <<"Error storing EXR file :( --> " << err;
        return 0;
    }

    

}

