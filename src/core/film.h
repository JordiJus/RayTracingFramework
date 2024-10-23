#ifndef FILM_H
#define FILM_H

#include "vector3d.h"
#include "bitmap.h"

#include <iostream>


enum BufferImageFormat
{
    UNSIGNED_BYTE4,
    FLOAT4,
    FLOAT3
};

struct ImageBufferEXR
{
    void* data = nullptr;
    unsigned int      width = 0;
    unsigned int      height = 0;
    BufferImageFormat pixel_format;
    // The memory backed by data isn't always owned by ImageBuffer (e.g. in the case of
    // loadImage), so you can't always free the memory in a destructor. Additionally you
    // can't simply delete the memory in the client either, because on some systems the
    // heap isn't shared between the sutil library and the client. In this case you should
    // call destroy to free the memory.
    void destroy();
};

/**
 * @brief The Film class
 */
class Film
{
public:
    // Constructor(s)
    Film(size_t width_, size_t height_);
    Film() = delete;

    // Destructor
    ~Film();

    // Getters
    size_t getWidth() const;
    size_t getHeight() const;
    Vector3D getPixelValue(size_t w, size_t h) const;

    // Setters
    void setPixelValue(size_t w, size_t h, Vector3D &value);

    // Other functions
    int save();
    int saveEXR();
    void clearData();

private:
    // Image size
    size_t width;
    size_t height;

    // Pointer to image data
    Vector3D **data;
};

#endif // FILM_H
