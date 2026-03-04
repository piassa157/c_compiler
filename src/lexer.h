#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
    // Keywords
    TOK_INT,
    TOK_RETURN,
    // Indetifiers / literals
    TOK_IDENT,
    TOK_INT_LIT,

    //Symbols

    TOK_LPAREN, //(
    TOK_RPAREN, //)
    TOK_LBRACE, //{
    TOK_RBRACE, //}
    TOK_SEMI, //;
    TOK_PLUS, //+
    TOK_MINUS, //-
    TOK_STAR, //*
    TOK_SLASH, // /
    TOK_EOF,

    TOK_INVALID
} TokenKind;


typedef struct {
    TokenKind kind;
    const char* start;
    size_t length;
    int int_value; 
    size_t line;
    size_t col;
} Token;


typedef struct {
    const char* src;
    size_t len;
    size_t pos;
    size_t line;
    size_t col;
} Lexer;

void lexer_init(Lexer* lx, const char* src, size_t len);
Token lexer_next(Lexer* lx);

const char* token_kind_str(TokenKind k);

#endif