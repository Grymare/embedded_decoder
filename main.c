#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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


    // Print all numbers
    for (int j = 0; j < 1023; j++) {
        printf("%ld\n", mySequence[j]);
    }


    return 0;
}