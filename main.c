#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <math.h>

const int NUMBER_OF_LENAS_SATELLITES = 24;
const int NUMBER_OF_BITS_IN_A_SEQUENCE = 1023;


void generateMotherSequenceOne(int mother[]) {
    int sequence[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int indices[2] = {2, 9};

    printf("\nMutter 1: ");
    for (int k = 0; k < 2; k++) {
        printf("%d", indices[k]);
    }

    for (int i = 0; i < 1023; i++) {
        int xorValue = 0;


        for (int j = 0; j < 2; j++) {
            xorValue += sequence[indices[j]];
        }
        mother[i] = sequence[9];
        printf("%d", mother[i]);

        // Shift all values to the right
        for (int k = 9; k > 0; k--) {
            sequence[k] = sequence[k - 1];
        }
        sequence[0] = xorValue % 2;
    }
}


void generateMotherSequenceTwo(int mother[]) {
    int sequence[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int indices[6] = {1, 2, 5, 7, 8, 9};

    printf("\nMutter 2: ");
    for (int k = 0; k < 6; k++) {
        printf("%d", indices[k]);
    }

    for (int i = 0; i < 1023; i++) {
        int xorValue = 0;


        for (int j = 0; j < 6; j++) {
            xorValue += sequence[indices[j]];
        }
        mother[i] = sequence[9];
        printf("%d", mother[i]);

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

}


void getSequenceFromTxtFile(char *filename, int *sequence) {
    int i = 0;
    for (int j = 0; j < 2; ++j) {
        FILE *ptr_file;

        char buffer[1000];

        printf("Auszulesende Datei: %s\n\n", filename);

        ptr_file = fopen(filename, "r");


        if (!ptr_file) {
            printf("ERROR");
        }


        while (fgets(buffer, 1000, ptr_file) != NULL) {
//            printf("%s\n", buffer);

            char *p = buffer;
            while (*p) { // While there are more characters to process...
                if (isdigit(*p) || ((*p == '-' || *p == '+') && isdigit(*(p + 1)))) {
                    // Found a number
                    long val = strtol(p, &p, 10); // Read number
                    sequence[i++] = val;

                } else {
                    // Otherwise, move on to the next character.
                    p++;
                }
            }
        }

        fclose(ptr_file);
    }
}

int scalarProductCalculation(int *goldCode, int *gpsSequence) {
    int scalarProduct = 0;
//    printf("\nGPSSequenz: ");
    for (int i = 0; i < 1023; ++i) {
//        printf("%d", gpsSequence[i]);
        scalarProduct += goldCode[i] * gpsSequence[i];
    }
    return scalarProduct;
}

compareSequences(int goldCodes[24][1023], char *filename) {
    int gpsSequence[2046];
    getSequenceFromTxtFile(filename, gpsSequence);

    printf("\nGps aus Datei: ");
    for (int k = 0; k < 2046; ++k) {
        printf("%d", gpsSequence[k]);
    }

    printf("\nGold 17:");
    for (int i = 0; i < 1023; ++i) {
        printf("%d", goldCodes[16][i]);

    }


    printf("\n\n");

    for (int i = 0; i < 24; ++i) {
        float maxScalar = 0.0;
        int offsetWithMaxScalar = -1;

        for (int j = 0; j < 1023; ++j) {

            int scalarProduct = scalarProductCalculation(goldCodes[i], gpsSequence + j);
            float scalarFixed = scalarProduct / 1023.0;
            if (fabsf(scalarFixed) > fabsf(maxScalar)) {
                maxScalar = scalarFixed;
                offsetWithMaxScalar = j;
            }
        }
//        printf("\n Satellit %d hat maxScalar: %f", i + 1, maxScalar);
        if (maxScalar >= 0.8) {
            printf("\n\nSatellite %d has sent bit %d (delta = %d)", i + 1, 1, offsetWithMaxScalar);
            printf("\n Scalar: %f", maxScalar);
        } else if (maxScalar <= -0.8) {
            printf("\n\nSatellite %d has sent bit %d (delta = %d)", i + 1, 0, offsetWithMaxScalar);
            printf("\n Scalar: %f", maxScalar);
        }

    }

}


int decode(char *filename) {
    int goldCodes[24][1023] = {0};

    int firstMotherSequence[1023];
    int secondMotherSequence[1023];


    generateMotherSequenceOne(firstMotherSequence);
    generateMotherSequenceTwo(secondMotherSequence);

    generateGoldCodes(firstMotherSequence, secondMotherSequence, goldCodes);


    compareSequences(goldCodes, filename);

}


int main(int argc, char *argv[]) {
    decode(argv[1]);

    return 0;
}
