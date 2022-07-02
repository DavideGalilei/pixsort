#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define MAX_POINTS 100
int points[MAX_POINTS];

#define MAX_X_DEPTH 200
#define MAX_Y_DEPTH 100


static inline int randint(int M, int N) {
    // https://stackoverflow.com/a/1202904
    return M + rand() / (RAND_MAX / (N - M + 1) + 1);
}


static inline int min(int a, int b) {
    return a > b ? b : a;
}


void pixel_sort(unsigned char *data, int x, int y, int channels, int point) {
    int p_x = point / y;
    int p_y = point % x;

    int x_depth = randint(0, min(x - p_x, MAX_X_DEPTH));

    for (int x_off = 0; x_off < x_depth; x_off++) {
        void *copy_me = &data[p_y * x * channels + (p_x + x_off) * channels];
        int y_depth = randint(0, min(y - p_y, MAX_Y_DEPTH));

        for (int y_off = 0; y_off < y_depth; y_off++) {
            memcpy(
                &data[(p_y + y_off) * x * channels + (p_x + x_off) * channels],
                copy_me,
                channels
            );
        }
    }
}


int main(int argc, char *argv[]) {
    char *executable = *argv++;

    if (argc < 2) {
        printf("You must provide an argument. Example: %s image.png\n", executable);
        return (EXIT_FAILURE);
    }

    char *filename = *argv++;
    int x, y, channels;
    unsigned char *data = stbi_load(filename, &x, &y, &channels, 0);

    if (!data) {
        printf("An error occurred while loading the image\n");
        return (EXIT_FAILURE);
    }

    printf("Loaded `%s` (width: %d | height: %d)\n",
            filename, x, y);

    srand(time(NULL));

    for (size_t i = 0; i < MAX_POINTS; i++) {
        // choose random pixels within the image
        points[i] = randint(0, x * y);
    }

    for (size_t i = 0; i < MAX_POINTS; i++) {
        pixel_sort(data, x, y, channels, points[i]);
    }

    const char suffix[] = "sorted.png";
    const int filename_len = strlen(filename) + 1;

    char *output_file = malloc(filename_len + sizeof(suffix));
    memcpy(output_file, filename, filename_len);
    strcat(output_file, suffix);

    printf("Writing image to `%s`...\n", output_file);

    if (!stbi_write_png(output_file, x, y, channels, data, x * channels)) {
        printf("An error occurred while writing the image to `%s`\n", output_file);
        return (EXIT_FAILURE);
    }

    stbi_image_free(data);
    return (EXIT_SUCCESS);
}
