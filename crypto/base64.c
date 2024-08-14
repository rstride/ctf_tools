#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

void base64_decode(const char *input, char *output, int *output_len) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bio_mem = BIO_new_mem_buf(input, -1);
    bio_mem = BIO_push(b64, bio_mem);
    *output_len = BIO_read(bio_mem, output, strlen(input));
    output[*output_len] = '\0';
    BIO_free_all(bio_mem);
}

int main(char **argv) {
    const char *input = argv[1];
    char output[256];
    int output_len;

    base64_decode(input, output, &output_len);

    printf("Decoded: %s\n", output);
    
    return 0;
}