#include <stdio.h>

#define VERSION "0.1.0"

int main(int argc, char *argv[]) {
    printf("ASCII Art Converter v%s\n", VERSION);
    
    if (argc < 2) {
        printf("Usage: %s <image-file>\n", argv[0]);
        return 1;
    }
    
    printf("Input file: %s\n", argv[1]);
    printf("(Image loading not yet implemented)\n");
    
    return 0;
}
