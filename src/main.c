#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static char* read_file(const char* path, size_t* out_len){
    FILE* f = fopen(path, "rb");
    if(!f){
        perror("fopen");
        exit(1);
    }


    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = malloc(size + 1);
    fread(buffer, 1, size,f);
    fclose(f);

    buffer[size] = '\0';

    if(out_len) *out_len = size;
    return buffer;
}

static int is_stage_option(const char* arg){
    return strcmp(arg, "--lex") == 0 ||
           strcmp(arg, "--parse") == 0 ||
           strcmp(arg, "--validate") == 0 ||
           strcmp(arg, "--tacky") == 0 ||
           strcmp(arg, "--codegen") == 0;
}

int main(int argc, char** argv){
    const char* input_path = NULL;

    for(int i = 1; i < argc; i++){
        const char* arg = argv[i];
        if(arg[0] == '-'){
            if(!is_stage_option(arg)){
                fprintf(stderr, "unknown option: %s\n", arg);
                return 1;
            }
            continue;
        }
        if(input_path != NULL){
            fprintf(stderr, "multiple input files are not supported\n");
            return 1;
        }
        input_path = arg;
    }

    if(input_path == NULL){
        fprintf(stderr, "usage: %s [--lex|--parse|--validate|--tacky|--codegen] file.c\n", argv[0]);
        return 1;
    }


    size_t len;
    char* src = read_file(input_path, &len);

    Lexer lexer;
    lexer_init(&lexer, src, len);

    while(1){
        Token tok = lexer_next(&lexer);

        if(tok.kind == TOK_INVALID){
            fprintf(stderr, "lex error at line %zu, col %zu: '%.*s'\n", tok.line, tok.col, (int)tok.length, tok.start);
            free(src);
            return 1;
        }

        if(tok.kind == TOK_EOF)
            break;
    }

    free(src);
    return 0;
}
