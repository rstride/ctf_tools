#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hex_to_ascii(const char *hex, char *ascii) {
    for (int i = 0; i < strlen(hex); i += 2) {
        unsigned int byte;
        sscanf(hex + i, "%2x", &byte);
        ascii[i/2] = byte;
    }
    ascii[strlen(hex)/2] = '\0';  
}

void xor(const char *key1, const char *key2, const char *key3, const char *Result, char *output) {
    for (int i = 0; i < strlen(key1); i += 2) {
        unsigned int val1, val2, val3, valResult;
        sscanf(key1 + i, "%2x", &val1);
        sscanf(key2 + i, "%2x", &val2);
        sscanf(key3 + i, "%2x", &val3);
        sscanf(Result + i, "%2x", &valResult);
        
        unsigned int val_password = val1 ^ val2 ^ val3 ^ valResult;
        sprintf(output + i, "%02x", val_password);
    }
}

int main() {
    const char *Key1 = "1039380a3d3c0d0028465f0b3b016d704c1333193e7a12205a2d0812";
    const char *Key2 = "796a6d440c6a583705213558577159231276103c074e715469665a3c";
    const char *Key3 = "29011f095c24234c5654580723410665231874417a1e38121928237d";
    const char *Result = "086744430f47467f12625875283534244866180b040a4e013176744e";
    
    char Password[100];
    char ascii[strlen(Result)/2 + 1];

    xor(Key1, Key2, Key3, Result, Password);
    hex_to_ascii(Password, ascii);

    printf("Hexa password: %s\n", Password);
    printf("ASCII password: %s\n", ascii);
    
    return 0;
}
