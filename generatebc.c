// generatebc.c - Program to generate barcode PBM images
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "barcode.h"

int main(int argc, char *argv[]) {
    // Check if barcode number was provided
    if (argc < 2) {
        printf("Error: Please provide an 8-digit barcode number.\n");
        return 1;
    }

    // Get parameters from command line arguments
    char *barcode_num = argv[1];
    int margin_size = (argc > 2) ? atoi(argv[2]) : 4;      // Default margin: 4 pixels
    int bar_width = (argc > 3) ? atoi(argv[3]) : 3;        // Default bar width: 3 pixels
    int bar_height = (argc > 4) ? atoi(argv[4]) : 50;      // Default height: 50 pixels
    char *output_path = (argc > 5) ? argv[5] : "barcode_output.pbm";

    // Validate barcode length
    if (strlen(barcode_num) < 8) {
        printf("Error: Barcode must be 8 digits long.\n");
        return 1;
    }

    // Validate barcode checksum
    if (!validate_barcode(barcode_num)) {
        printf("Error: Invalid barcode checksum.\n");
        return 1;
    }

    // Check if output file already exists
    if (check_file_exists(output_path)) {
        char user_choice;
        printf("File '%s' exists. Overwrite? (y/n): ", output_path);
        scanf(" %c", &user_choice);
        if (user_choice != 'y' && user_choice != 'Y') {
            printf("Error: Operation cancelled.\n");
            return 1;
        }
    }

    // Generate the barcode image
    create_barcode_image(barcode_num, margin_size, bar_width, bar_height, output_path);
    printf("Barcode image created successfully: '%s'.\n", output_path);

    return 0;
}