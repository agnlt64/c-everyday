#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdio.h>

void img_to_ascii(unsigned char* image, int width, int height, int channels)
{
    const char* density = " .:-i|=+%O#@";

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            unsigned char* pixel = image + (y * width + x) * channels;
            unsigned char r = pixel[0];
            unsigned char g = pixel[1];
            unsigned char b = pixel[2];

            float luminance = 0.2126f * r + 0.7152f * g + 0.0722f * b;
            int index = (int)(luminance / 255.0f * (sizeof(density) - 1));
            printf("%c", density[index]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    const char* img_path = argv[1];
    int w, h, ch;
    unsigned char* image = stbi_load(img_path, &w, &h, &ch, 0);

    if (image == NULL)
    {
        fprintf(stderr, "Failed to load image `%s`\n", img_path);
        return 1;
    }

    img_to_ascii(image, w, h, ch);

    stbi_image_free(image);

    return 0;
}