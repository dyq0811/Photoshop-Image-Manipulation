//testfile
#include <stdio.h>
#include <assert.h>
#include <string.h>

/*
 * Reads lhs and rhs character by character until either reaches eof.
 * Returns true if the files can be read and the two files match
 * character by character. Returns false if two files either can't be
 * opened or don't match.
 */
int fileeq(char lhsName[], char rhsName[]) {
    FILE* lhs = fopen(lhsName, "rb");
    FILE* rhs = fopen(rhsName, "rb");
    
    // don't compare if we can't open the files
    if (lhs == NULL || rhs == NULL) return 0;

    int n = 100; // max size
    char tag1[n];
    char tag2[n];

    fgets(tag1, n, lhs);
    fgets(tag2, n, rhs);
    assert(strcmp(tag1, "P6\n") == 0);
    assert(strcmp(tag2, "P6\n") == 0);
    
    fgets(tag1, n, lhs);
    if (tag1[0] == '#')
      fgets(tag1, n, lhs);

    fgets(tag2, n, rhs);
    if (tag2[0] == '#')
      fgets(tag2, n, rhs);
    
    int c1 = 0;
    int r1 = 0;
    sscanf(tag1, "%d %d", &c1, &r1);
    assert(c1 * r1 != 0);

    int c2 = 0;
    int r2 = 0;
    sscanf(tag2, "%d %d", &c2, &r2);
    assert(c2 * r2 != 0);

    fgets(tag1, n, lhs);
    assert(strcmp(tag1, "255\n") == 0);

    fgets(tag2, n, rhs);
    assert(strcmp(tag2, "255\n") == 0);

    int match = 1;
    // read until both of the files are done or there is a mismatch
    while (!feof(lhs) || !feof(rhs)) {
        if (feof(lhs) ||                  // lhs done first
            feof(rhs) ||                  // rhs done first
            (fgetc(lhs) != fgetc(rhs))) { // chars don't match
            match = 0;
            break;
        }
    }
    fclose(lhs);
    fclose(rhs);
    return match;
}

void fileeqDisplay(char lhsName[], char rhsName[]) {
    FILE* lhs = fopen(lhsName, "rb");
    FILE* rhs = fopen(rhsName, "rb");
    

    int n = 100; // max size
    char tag1[n];
    char tag2[n];

    fgets(tag1, n, lhs);
    fgets(tag2, n, rhs);
    assert(strcmp(tag1, "P6\n") == 0);
    assert(strcmp(tag2, "P6\n") == 0);
    
    fgets(tag1, n, lhs);
    if (tag1[0] == '#')
      fgets(tag1, n, lhs);

    fgets(tag2, n, rhs);
    if (tag2[0] == '#')
      fgets(tag2, n, rhs);
    
    int c1 = 0;
    int r1 = 0;
    sscanf(tag1, "%d %d", &c1, &r1);
    assert(c1 * r1 != 0);

    int c2 = 0;
    int r2 = 0;
    sscanf(tag2, "%d %d", &c2, &r2);
    assert(c2 * r2 != 0);

    fgets(tag1, n, lhs);
    assert(strcmp(tag1, "255\n") == 0);

    fgets(tag2, n, rhs);
    assert(strcmp(tag2, "255\n") == 0);

    // print mismatched characters
    while (!feof(lhs) || !feof(rhs)) {
        char output1 = fgetc(lhs);
        char output2 = fgetc(rhs);
        if (feof(lhs) ||                  // lhs done first
            feof(rhs) ||                  // rhs done first
            (output1 != output2)) { // chars don't match
            printf("%d and %d\n", output1, output2);
        }
    }
    fclose(lhs);
    fclose(rhs);
}


int main(int argc, char *argv[]) {
  assert(argc == 3);
  if (fileeq(argv[1], argv[2]) == 0) {
        printf("Files not equal\n");
    }
    else {
        printf("Files are equal\n");
    }
    return 0;
  /*    fileeqDisplay(argv[1], argv[2]);*/
}
