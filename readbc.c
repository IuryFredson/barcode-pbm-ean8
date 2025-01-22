// readbc.c - Program to read barcodes from PBM images
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "barcode.h"

int main(int argc, char *argv[]) {
    // Check if input file was provided
    if (argc < 2) {
        printf("Error: Please provide a PBM file path.\n");
        return 1;
    }

    // Get input file path from command line arguments
    char *input_path = argv[1];

    // Verify file exists
    if (!check_file_exists(input_path)) {
        printf("Error: File '%s' does not exist.\n", input_path);
        return 1;
    }

    // Buffer to store the decoded barcode
    char barcode_result[9];

    // Attempt to read barcode from image
    if (!read_barcode_from_image(input_path, barcode_result)) {
        printf("Error: Could not read barcode from file '%s'.\n", input_path);
        return 1;
    }

    // Print the successfully decoded barcode
    printf("Decoded barcode: %s\n", barcode_result);
    return 0;
}