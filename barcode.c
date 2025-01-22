// barcode.c - Implementation of barcode operations
#include "barcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Encoding patterns for left-side digits
const char *left_patterns[10] = {
    "0001101", "0011001", "0010011", "0111101",
    "0100011", "0110001", "0101111", "0111011",
    "0110111", "0001011"
};

// Encoding patterns for right-side digits
const char *right_patterns[10] = {
    "1110010", "1100110", "1101100", "1000010",
    "1011100", "1001110", "1010000", "1000100",
    "1001000", "1110100"
};
// Static function to read a single bit based on pixel data
// Reads the intensity of pixels over a "bar_width" span, interpreting as 0 or 1
static int read_bit(const int *pixel_data, int width, int middle_row, int pos, int bar_width) {
    int sum = 0;
    for (int i = 0; i < bar_width; i++) {
        if (pos + i < width) { // Ensure we stay within the image width
            sum += pixel_data[middle_row * width + pos + i]; // Accumulate pixel values
        }
    }
    return (sum > bar_width / 2) ? 1 : 0; // Return 1 if the majority is "dark"
}

// Static function to read a 7-bit pattern
// Uses `read_bit` to gather each bit of the pattern and builds the pattern string
static void read_pattern(const int *pixel_data, int width, int middle_row, int pos, int bar_width, char *pattern) {
    for (int i = 0; i < 7; i++) {
        // Read each bit and append to the pattern string
        pattern[i] = '0' + read_bit(pixel_data, width, middle_row, pos + i * bar_width, bar_width);
    }
    pattern[7] = '\0'; // Null-terminate the pattern string
}

bool validate_barcode(const char *barcode_num) {
    // Check length
    if (strlen(barcode_num) != 8) return false;

    // Verify all characters are digits
    for (int i = 0; i < 7; i++) {
        if (!isdigit(barcode_num[i])) return false;
    }

    // Calculate checksum
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        int weight = (i % 2 == 0) ? 3 : 1;
        sum += weight * (barcode_num[i] - '0');
    }

    // Verify check digit
    int check_digit = (10 - (sum % 10)) % 10;
    return (check_digit == barcode_num[7] - '0');
}

bool check_file_exists(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

void create_barcode_image(const char *barcode_num, int margin_size, int bar_width, int bar_height, const char *output_path) {
    FILE *output_file = fopen(output_path, "w");
    if (!output_file) {
        printf("Error: Cannot create file %s.\n", output_path);
        return;
    }

    // Calculate image dimensions
    int barcode_width = (3 + 28 + 5 + 28 + 3) * bar_width + 2 * margin_size;
    int total_height = bar_height + 2 * margin_size;

    // Write PBM header
    fprintf(output_file, "P1\n%d %d\n", barcode_width, total_height);

    // Generate image data
    for (int y = 0; y < total_height; y++) {
        for (int x = 0; x < barcode_width; x++) {
            // Handle margins
            if (y < margin_size || y >= total_height - margin_size ||
                x < margin_size || x >= barcode_width - margin_size) {
                fprintf(output_file, "0 ");
                continue;
            }

            int bar_pos = (x - margin_size) / bar_width;
            int pixel_value = 0;

            // Generate start pattern
            if (bar_pos < 3) {
                pixel_value = (bar_pos == 0 || bar_pos == 2) ? 1 : 0;
            }
            // Generate left side pattern
            else if (bar_pos < 3 + 28) {
                int digit_index = (bar_pos - 3) / 7;
                int pattern_pos = (bar_pos - 3) % 7;
                pixel_value = left_patterns[barcode_num[digit_index] - '0'][pattern_pos] - '0';
            }
            // Generate middle pattern
            else if (bar_pos < 3 + 28 + 5) {
                pixel_value = ((bar_pos - (3 + 28)) % 2 == 0) ? 0 : 1;
            }
            // Generate right side pattern
            else if (bar_pos < 3 + 28 + 5 + 28) {
                int digit_index = 4 + (bar_pos - (3 + 28 + 5)) / 7;
                int pattern_pos = (bar_pos - (3 + 28 + 5)) % 7;
                pixel_value = right_patterns[barcode_num[digit_index] - '0'][pattern_pos] - '0';
            }
            // Generate end pattern
            else {
                pixel_value = (bar_pos == 3 + 28 + 5 + 28 || bar_pos == 3 + 28 + 5 + 28 + 2) ? 1 : 0;
            }

            fprintf(output_file, "%d ", pixel_value);
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);
}

////// READ


bool read_barcode_from_image(const char *image_path, char *result) {
    FILE *input_file = fopen(image_path, "r");
    if (!input_file) {
        printf("Error: Cannot open file '%s'.\n", image_path);
        return false;
    }

    // Read PBM header
    char format[3];
    int width, height;
    fscanf(input_file, "%s %d %d", format, &width, &height);

    // Verify file format
    if (strcmp(format, "P1") != 0) {
        printf("Error: File is not in ASCII PBM format.\n");
        fclose(input_file);
        return false;
    }

    // Allocate memory for pixel data
    int *pixel_data = malloc(width * height * sizeof(int));
    if (!pixel_data) {
        printf("Error: Out of memory.\n");
        fclose(input_file);
        return false;
    }

    // Read pixel data
    for (int i = 0; i < width * height; i++) {
        fscanf(input_file, "%d", &pixel_data[i]);
    }
    fclose(input_file);

    // Decode barcode from pixel data
    bool success = decode_from_pixels(pixel_data, width, height, result);
    free(pixel_data);
    return success;
}


// Function to decode the barcode from the pixel data
// Processes the PBM image to extract and validate the barcode number
bool decode_from_pixels(const int *pixel_data, int width, int height, char *result) {
    if (!pixel_data || !result || width <= 0 || height <= 0) {
        return false; // Invalid parameters
    }

    int middle_row = height / 2; // Focus on the middle row for horizontal alignment
    int bar_width = 0;

    // Find the bar width by locating the first black bar and measuring its width
    int x = 0;
    while (x < width && pixel_data[middle_row * width + x] == 0) x++; // Skip white space
    int start_black = x; // Start of the first black bar
    while (x < width && pixel_data[middle_row * width + x] == 1) x++; // Count the black bar width
    bar_width = x - start_black;

    if (bar_width == 0) return false; // No bar found

    // Locate the starting position of the barcode (Start Guard pattern)
    int start_pos = -1;
    for (x = 0; x < width - bar_width * 3; x++) {
        // Check for "101" pattern to identify the Start Guard
        if (read_bit(pixel_data, width, middle_row, x, bar_width) == 1 &&
            read_bit(pixel_data, width, middle_row, x + bar_width, bar_width) == 0 &&
            read_bit(pixel_data, width, middle_row, x + 2 * bar_width, bar_width) == 1) {
            start_pos = x; // Found start position
            break;
        }
    }

    if (start_pos == -1) return false; // Start pattern not found

    char decoded[9] = {0};
    int decoded_pos = 0;
    start_pos += 3 * bar_width; // Skip the Start Guard pattern

    char pattern[8];

    // Decode the left-hand side (4 digits)
    for (int digit = 0; digit < 4; digit++) {
        read_pattern(pixel_data, width, middle_row, start_pos, bar_width, pattern); // Read pattern
        bool found = false;

        // Match the pattern with left-side encoding
        for (int i = 0; i < 10; i++) {
            if (strcmp(pattern, left_patterns[i]) == 0) {
                decoded[decoded_pos++] = '0' + i; // Append digit to result
                found = true;
                break;
            }
        }

        if (!found) {
            return false; // Invalid pattern
        }

        start_pos += 7 * bar_width; // Move to the next pattern
    }

    // Skip the Middle Guard pattern (01010)
    start_pos += 5 * bar_width;

    // Decode the right-hand side (4 digits)
    for (int digit = 0; digit < 4; digit++) {
        read_pattern(pixel_data, width, middle_row, start_pos, bar_width, pattern); // Read pattern
        bool found = false;

        // Match the pattern with right-side encoding
        for (int i = 0; i < 10; i++) {
            if (strcmp(pattern, right_patterns[i]) == 0) {
                decoded[decoded_pos++] = '0' + i; // Append digit to result
                found = true;
                break;
            }
        }

        if (!found) {
            return false; // Invalid pattern
        }

        start_pos += 7 * bar_width; // Move to the next pattern
    }

    decoded[8] = '\0'; // Null-terminate the decoded result

    // Validate the decoded barcode
    if (!validate_barcode(decoded)) {
        return false;
    }

    // Copy the decoded barcode to the result
    strcpy(result, decoded);
    return true;
    }