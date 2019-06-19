#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int NUMBER_OF_LENAS_SATELLITES = 24;
int NUMBER_OF_BITS_IN_A_SEQUENCE = 1023;


void generateMotherSequence(int mother[], int indices[]) {
    int sequence[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (int i = 0; i < NUMBER_OF_BITS_IN_A_SEQUENCE; i++) {
        int xorValue = 0;


        for (int j = 0; j < sizeof(indices)/ sizeof(int); j++) {
            xorValue += sequence[indices[j]];
        }
        mother[i] = sequence[9];

        // Shift all values to the right
        for (int k = 9; k > 0; k--) {
            sequence[k] = sequence[k - 1];
        }
        sequence[0] = xorValue % 2;
    }
}

int decode() {

    printf("decode");
    int goldCodes[24][1023] = {0};

    int firstMotherSequence[1023];
    int secondMotherSequence[1023];

    int firstIndices[2] = {2, 9};
    int secondIndices[6] = {1, 2, 5, 7, 8, 9};

    generateMotherSequence(firstMotherSequence, firstIndices);

    for (int i = 0; i < sizeof(firstMotherSequence); i++) {
        printf("%d ", firstMotherSequence[i]);
        printf("\n");
    }

//    generateMotherSequence(secondMotherSequence, secondIndices);


}


int main(int argc, char *argv[]) {
    char *filename = argv[1];
    int i = 0;
    FILE *ptr_file;

    char buffer[1000];
    long mySequence[1023];

    printf("Auszulesende Datei: %s\n\n", filename);

    ptr_file = fopen(filename, "r");


    if (!ptr_file) {
        return 1;
    }

    while (fgets(buffer, 1000, ptr_file) != NULL) {
        printf("%s\n", buffer);

        char *p = buffer;
        while (*p) { // While there are more characters to process...
            if (isdigit(*p) || ((*p == '-' || *p == '+') && isdigit(*(p + 1)))) {
                // Found a number
                long val = strtol(p, &p, 10); // Read number
                mySequence[i++] = val;

            } else {
                // Otherwise, move on to the next character.
                p++;
            }
        }
    }

    fclose(ptr_file);


//    // Print all numbers
//    for (int j = 0; j < 1023; j++) {
//        printf("%ld\n", mySequence[j]);
//    }

    printf("test");

    decode();


    return 0;
}
