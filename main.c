#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

const int NUMBER_OF_LENAS_SATELLITES = 24;
const int NUMBER_OF_BITS_IN_A_SEQUENCE = 1023;


void generateMotherSequence(int mother[], int indices[]) {
    int sequence[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    printf("generator");

    for (int i = 0; i < NUMBER_OF_BITS_IN_A_SEQUENCE; i++) {
        int xorValue = 0;

        for (int j = 0; indices[j] != 0; j++) {
            xorValue += sequence[indices[j]];
            printf("indices: %d", indices[j]);
        }
        mother[i] = sequence[9];

        // Shift all values to the right
        for (int k = 9; k > 0; k--) {
            sequence[k] = sequence[k - 1];
        }
        sequence[0] = xorValue % 2;
    }
}


void rotateByOne(int arr[]) {
    int i, last;

    last = arr[1022];

    for (i = 1022; i > 0; i--) {
        arr[i] = arr[i - 1];
    }

    arr[0] = last;
}

void rotateArray(int arr[], int t) {
    for (int j = 0; j < t; ++j) {
        rotateByOne(arr);
    }
}


void generateGoldCodes(int firstMotherSequence[], int secondMotherSequence[], int goldCodes[24][1023]) {
    int t[] = {5, 6, 7, 8, 17, 18, 139, 140, 141, 251, 252, 254, 255,
               256, 257, 258, 469, 470, 471, 472, 473, 474, 509, 512};

    for (int i = 0; i < NUMBER_OF_LENAS_SATELLITES; i++) {
        int workingSequence[1023];
        memcpy(workingSequence, secondMotherSequence, 1023 * sizeof(int));

        rotateArray(workingSequence, t[i]);

        // If the XOR product is 0 we write a -1 in that position otherwise we simply put a 1 there
        for (int k = 0; k < NUMBER_OF_BITS_IN_A_SEQUENCE; ++k) {
            goldCodes[i][k] = ((firstMotherSequence[k] + workingSequence[k]) % 2) == 0 ? -1 : 1;
        }
    }

    printf("\nGOLD: ");
    for (int i = 0; i < 1023; ++i) {
        printf("%d", goldCodes[0][i]);
    }


}


int decode() {
    int goldCodes[24][1023] = {0};

    int firstMotherSequence[1023];
    int secondMotherSequence[1023];

    int firstIndices[2] = {2, 9};
    int secondIndices[6] = {1, 2, 5, 7, 8, 9};

    generateMotherSequence(firstMotherSequence, firstIndices);
    generateMotherSequence(secondMotherSequence, secondIndices);

    generateGoldCodes(firstMotherSequence, secondMotherSequence, goldCodes);

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
//    for (int j = 0; j < NUMBER_OF_BITS_IN_A_SEQUENCE; j++) {
//        printf("%ld\n", mySequence[j]);
//    }

    printf("test");

    decode();


    return 0;
}
