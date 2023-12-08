
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct Data {
} Data;

typedef struct Code {
    Data* (*encode)(void*, Data*);
    Data* (*decode)(void*, Data*);
    void* info;
    bool initialized;
} Code;

Data* encode(Code* code, Data* data) {
    if (code->initialized) {
        return code->encode(code->info, data);
    } else {
        printf("NOT INIALIZED\n");
    }
    return NULL;
}

Data* decode(Code* code, Data* data) {
    return code->decode(code->info, data);
}

// ------ AES.h --------

typedef struct {
} AES_config;

// ------ AES.c

Data* aes_encode(void* config, Data* input) {
    printf("AES ENCODE CALLED\n");
    return NULL;
}

Data* aes_decode(void* config, Data* input) {
    printf("AES DECODE CALLED\n");
    return NULL;
}

void aes(Code* code, AES_config* config) {
    code->encode = aes_encode;
    code->decode = aes_decode;
    code->info = config;
}

int main(int argc, char* argv[]) {
    Code code = {0};

    if (argc > 1 && strcmp(argv[1], "aes") == 0) {
        AES_config config;
        aes(&code, &config);
    }

    Data* data;
    Data* encoded = encode(&code, data);
}
