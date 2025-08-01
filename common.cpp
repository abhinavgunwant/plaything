#include "common.hpp"

void resizeImage(int width, int height, Image &image) {
    if (image.width != width) { ImageResize(&image, width, image.height); }
    if (image.height != height) { ImageResize(&image, image.width, height); }
}

Texture2D imageToTexture(int width, int height, Image image) {
    resizeImage(width, height, image);

    return LoadTextureFromImage(image);
}

