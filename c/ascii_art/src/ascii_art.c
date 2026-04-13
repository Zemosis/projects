#include "../include/ascii.h"
#include "../include/image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define VERSION "0.2.0"

static int terminal_get_width(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        return ws.ws_col;
    }
#endif
    return 80;
}

static void print_help(const char *prog) {
    printf("ASCII Art Converter v%s\n\n", VERSION);
    printf("Usage: %s <image-file> [options]\n\n", prog);
    printf("Options:\n");
    printf("  --width N       Set output width in characters\n");
    printf("  --scale N       Set downsampling factor\n");
    printf("  --no-color      Disable ANSI color codes\n");
    printf("  -o <file>       Save ASCII art to file\n");
    printf("  -h, --help      Show this help message\n");
    printf("\nBy default, output auto-fits to 50%% of terminal width.\n");
    printf("Priority: --width > --scale > auto-detect\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    /* Parse arguments */
    const char *image_file = NULL;
    const char *output_file = NULL;
    int use_color = 1;
    int user_width = 0;
    int user_scale = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--no-color") == 0) {
            use_color = 0;
        } else if (strcmp(argv[i], "--width") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --width requires a value\n");
                return 1;
            }
            user_width = atoi(argv[++i]);
            if (user_width < 1) {
                fprintf(stderr, "Error: --width must be >= 1\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--scale") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: --scale requires a value\n");
                return 1;
            }
            user_scale = atoi(argv[++i]);
            if (user_scale < 1) {
                fprintf(stderr, "Error: --scale must be >= 1\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: -o requires a filename\n");
                return 1;
            }
            output_file = argv[++i];
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            return 1;
        } else if (!image_file) {
            image_file = argv[i];
        } else {
            fprintf(stderr, "Error: unexpected argument '%s'\n", argv[i]);
            return 1;
        }
    }

    if (!image_file) {
        fprintf(stderr, "Error: no image file specified\n");
        return 1;
    }

    /* Load image */
    printf("Loading image: %s\n", image_file);
    Image *img = image_load(image_file);
    if (!img) {
        fprintf(stderr, "Error: Could not load image\n");
        return 1;
    }

    printf("Image: %dx%d (%d channels)\n", img->width, img->height,
           img->channels);

    /* Compute output dimensions */
    int target_w;
    if (user_width > 0) {
        target_w = user_width;
    } else if (user_scale > 0) {
        target_w = img->width / user_scale;
    } else {
        int term_cols = terminal_get_width();
        target_w = term_cols / 2;
    }

    if (target_w < 1) {
        target_w = 1;
    }

    /* Aspect ratio: terminal chars are ~2x taller than wide */
    int target_h = (target_w * img->height) / (img->width * 2);
    if (target_h < 1) {
        target_h = 1;
    }

    printf("Output size: %dx%d characters\n", target_w, target_h);

    /* Downscale and convert */
    Image *scaled = image_downscale(img, target_w, target_h);
    image_free(img);

    if (!scaled) {
        fprintf(stderr, "Error: downscale failed\n");
        return 1;
    }

    if (output_file) {
        printf("Writing ASCII art to: %s\n", output_file);
        if (ascii_write_to_file(scaled, output_file) == 0) {
            printf("Saved to '%s'\n", output_file);
        } else {
            fprintf(stderr, "Error: Failed to write to file\n");
            image_free(scaled);
            return 1;
        }
    } else {
        if (use_color) {
            printf("Color mode: enabled (use --no-color to disable)\n");
        }
        ascii_print_image(scaled, use_color);
    }

    image_free(scaled);
    return 0;
}
