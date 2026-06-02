#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

typedef struct {
    int width;
    int height;
    int maxValue;
    int channels;
    unsigned char *data;
} Image;

void freeImage(Image *img) {
    if (img->data != NULL) {
        free(img->data);
        img->data = NULL;
    }
}

int loadImage(const char *filename, Image *img) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    char format[3];

    if (fscanf(file, "%2s", format) != 1) {
        fclose(file);
        return 0;
    }

    if (strcmp(format, "P5") == 0) {
        img->channels = 1;
    } else if (strcmp(format, "P6") == 0) {
        img->channels = 3;
    } else {
        printf("Unsupported format. Use PGM (P5) or PPM (P6).\n");
        fclose(file);
        return 0;
    }

    int c = fgetc(file);

    while ((c = fgetc(file)) == '#') {
        while (fgetc(file) != '\n');
    }

    ungetc(c, file);

    fscanf(file, "%d %d", &img->width, &img->height);
    fscanf(file, "%d", &img->maxValue);
    fgetc(file);

    int imageSize = img->width * img->height * img->channels;

    img->data = (unsigned char *)malloc(imageSize);

    if (!img->data) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 0;
    }

    fread(img->data, 1, imageSize, file);
    fclose(file);

    return 1;
}

int saveImage(const char *filename, Image *img) {
    FILE *file = fopen(filename, "wb");

    if (!file) {
        printf("Cannot save image.\n");
        return 0;
    }

    if (img->channels == 1) {
        fprintf(file, "P5\n");
    } else {
        fprintf(file, "P6\n");
    }

    fprintf(file, "%d %d\n", img->width, img->height);
    fprintf(file, "%d\n", img->maxValue);

    int imageSize = img->width * img->height * img->channels;

    fwrite(img->data, 1, imageSize, file);

    fclose(file);

    return 1;
}

int compressImage(const char *inputFile, const char *outputFile) {
    Image img = {0};

    if (!loadImage(inputFile, &img)) {
        return 0;
    }

    FILE *out = fopen(outputFile, "wb");

    if (!out) {
        printf("Cannot create compressed file.\n");
        freeImage(&img);
        return 0;
    }

    fwrite(&img.width, sizeof(int), 1, out);
    fwrite(&img.height, sizeof(int), 1, out);
    fwrite(&img.maxValue, sizeof(int), 1, out);
    fwrite(&img.channels, sizeof(int), 1, out);

    int imageSize = img.width * img.height * img.channels;

    int i = 0;

    while (i < imageSize) {
        unsigned char current = img.data[i];
        unsigned int count = 1;

        while (i + count < imageSize &&
               img.data[i + count] == current &&
               count < 4294967295U) {
            count++;
        }

        fwrite(&count, sizeof(unsigned int), 1, out);
        fwrite(&current, sizeof(unsigned char), 1, out);

        i += count;
    }

    fclose(out);

    printf("Compression complete.\n");

    freeImage(&img);

    return 1;
}

int decompressImage(const char *compressedFile,
                    const char *outputImage) {

    FILE *in = fopen(compressedFile, "rb");

    if (!in) {
        printf("Cannot open compressed file.\n");
        return 0;
    }

    Image img = {0};

    fread(&img.width, sizeof(int), 1, in);
    fread(&img.height, sizeof(int), 1, in);
    fread(&img.maxValue, sizeof(int), 1, in);
    fread(&img.channels, sizeof(int), 1, in);

    int imageSize =
        img.width * img.height * img.channels;

    img.data =
        (unsigned char *)malloc(imageSize);

    if (!img.data) {
        fclose(in);
        return 0;
    }

    int index = 0;

    while (index < imageSize) {
        unsigned int count;
        unsigned char value;

        fread(&count, sizeof(unsigned int), 1, in);
        fread(&value, sizeof(unsigned char), 1, in);

        for (unsigned int i = 0;
             i < count && index < imageSize;
             i++) {
            img.data[index++] = value;
        }
    }

    fclose(in);

    if (!saveImage(outputImage, &img)) {
        freeImage(&img);
        return 0;
    }

    freeImage(&img);

    printf("Decompression complete.\n");

    return 1;
}

void printUsage() {
    printf("Usage:\n");
    printf("Compress:\n");
    printf("./program c input.ppm output.rle\n\n");

    printf("Decompress:\n");
    printf("./program d input.rle output.ppm\n");
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printUsage();
        return 1;
    }

    char mode = argv[1][0];

    if (mode == 'c') {
        compressImage(argv[2], argv[3]);
    }
    else if (mode == 'd') {
        decompressImage(argv[2], argv[3]);
    }
    else {
        printUsage();
    }

    return 0;
}