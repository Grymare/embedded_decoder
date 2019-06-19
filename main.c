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

int largest(int arr[], int n) {
    int i;
    int max = arr[0];
    for (i = 1; i < n; i++)
        if (arr[i] > max)
            max = arr[i];

    return max;
}

int smallest(int arr[], int n) {
    int i;
    int min = arr[0];
    for (i = 1; i < n; i++)
        if (arr[i] < min)
            min = arr[i];

    return min;
}

int getNumberOfSatellites(int sequence[]) {
    int max = largest(sequence, 1023);
    int min = smallest(sequence, 1023);

    int myOtherArray[] = {max, abs(min)};
    return largest(myOtherArray, 2);
}

void getSequenceFromTxtFile(char *filename, int sequence[]) {
    int i = 0;
    FILE *ptr_file;

    char buffer[1000];

    printf("Auszulesende Datei: %s\n\n", filename);

    ptr_file = fopen(filename, "r");


    if (!ptr_file) {
        printf("ERROR");
    }

    while (fgets(buffer, 1000, ptr_file) != NULL) {
        printf("%s\n", buffer);

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

int getScalarProduct(int sequence[1023], int sequenceTwo[1023]) {
    int scalarProduct = 0;
    for (int i = 0; i < 1023; ++i) {
        scalarProduct += sequence[i] * sequenceTwo[i];
    }
    return scalarProduct;
}

compareSequences(int goldCodes[24][1023], char *filename) {
    int gpsSequence[1023];
    getSequenceFromTxtFile(filename, gpsSequence);
    int numberOfSatellites = getNumberOfSatellites(gpsSequence);
    //TODO: 65?
    int maxMargin = 1023 - (numberOfSatellites * 65);
    int minMargin = (numberOfSatellites * 65) - 1023;

    for (int i = 0; i < 24; ++i) {
        int maxScalar = 0;
        int offsetWithMaxScalar = -1;

        for (int j = 0; j < 1023; ++j) {
            int gpsSequenceWithOffset[1023];
            memcpy(gpsSequenceWithOffset, gpsSequence, 1023 * sizeof(int));
            rotateArray(gpsSequenceWithOffset, j);

            int scalarProduct = getScalarProduct(goldCodes[i], gpsSequenceWithOffset);
            if (abs(scalarProduct) > abs(maxScalar)) {
                maxScalar = scalarProduct;
                offsetWithMaxScalar = j;
            }
            if (maxScalar >= maxMargin || maxScalar <= minMargin) {
                int bitValue = (scalarProduct > 1) ? 1 : 0;
                printf("\nSatellite %d has sent bit %d (delta = %d)", i + 1, bitValue, offsetWithMaxScalar);

            }

        }
        printf("\nmaxScalar %d", maxScalar);
        printf("   maxMargin %d", maxMargin);
        printf("   minMargin %d", minMargin);
        printf("\nFERTIG %d", i);
    }

}


int decode(char *filename) {
    int goldCodes[24][1023] = {0};

    int firstMotherSequence[1023];
    int secondMotherSequence[1023];

    int firstIndices[2] = {2, 9};
    int secondIndices[6] = {1, 2, 5, 7, 8, 9};

    generateMotherSequence(firstMotherSequence, firstIndices);
    generateMotherSequence(secondMotherSequence, secondIndices);

    generateGoldCodes(firstMotherSequence, secondMotherSequence, goldCodes);

    compareSequences(goldCodes, filename);

}


int main(int argc, char *argv[]) {
    decode(argv[1]);

    return 0;
}
