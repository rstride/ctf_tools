#include <stdio.h>
#include <string.h>

void hex_to_ascii(const char *hex, char *ascii) {
    for (int i = 0; i < strlen(hex); i += 2) {
        unsigned int byte;
        sscanf(hex + i, "%2x", &byte);
        ascii[i/2] = byte;
    }
    ascii[strlen(hex)/2] = '\0';
}

int main() {
    const char *hex = "48350e046235300469516a2167040612351b4f6f472015671b15051d";
    char ascii[strlen(hex)/2 + 1];
    
    hex_to_ascii(hex, ascii);
    
    printf("ASCII: %s\n", ascii);
    
    return 0;
}
