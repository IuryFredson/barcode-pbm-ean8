// barcode.h - Header file defining the interface for barcode operations
#ifndef BARCODE_H
#define BARCODE_H

#include <stdbool.h>

// Validates if a given barcode number follows the checksum rules
bool validate_barcode(const char *barcode_num);

// Checks if a file exists at the given path
bool check_file_exists(const char *filepath);

// Creates a PBM image file containing the barcode
void create_barcode_image(
    const char *barcode_num,     // 8-digit barcode number
    int margin_size,             // Size of white margins around barcode
    int bar_width,              // Width of each bar in pixels
    int bar_height,             // Height of the barcode
    const char *output_path     // Path where the PBM file will be saved
);

// Reads a barcode from a PBM image file
bool read_barcode_from_image(const char *image_path, char *result);

// Internal function to decode barcode from pixel data
bool decode_from_pixels(const int *pixel_data, int width, int height, char *result);

static int read_bit(const int *pixel_data, int width, int middle_row, int pos, int bar_width);
static void read_pattern(const int *pixel_data, int width, int middle_row, int pos, int bar_width, char *pattern);

#endif