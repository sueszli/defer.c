#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prevent compiler from optimizing away the overflow
__attribute__((noinline)) static void write_buffer(char *buf, int size) {
    for (int i = 0; i < size; i++) {
        buf[i] = 'A' + (char)(i % 26);
    }
    buf[size - 1] = '\0';
}

int main(void) {
    printf("Hello, World!\n");

    // Memory leak: allocate without freeing
    int *leaked = malloc(100 * sizeof(int));
    if (leaked) {
        leaked[0] = 42;
        printf("Allocated memory: %d\n", leaked[0]);
    }
    // Intentionally NOT freeing: free(leaked);

    // Buffer overflow: heap buffer overflow that ASAN will catch
    char *buffer = malloc(10);
    if (buffer) {
        // Write 20 bytes into a 10-byte buffer (overflow by 10 bytes)
        write_buffer(buffer, 20);
        printf("Buffer: %s\n", buffer);
        free(buffer);
    }

    return EXIT_SUCCESS;
}
