#include "lexer.h"
#include <ctype.h>
#include <string.h>

static char peek(Lexer* lx){
    if(lx->pos >= lx->len) return '\0';
    return lx->src[lx->pos];
}

static char advance(Lexer* lx){
    char c = peek(lx);
    if (c == '\0') return c;

    lx->pos++;
    if (c == '\n') { lx->line++; lx->col =1;}
    else {lx->col++;}
    return c;
}

static void skip_ws(Lexer* lx){
    while(1){
        char c = peek(lx);
        if(c == ' ' || c == '\t' || c == '\r' || c == "\n") advance(lx);
        else break;
    }
}

static Token make_tok(Lexer* lx, TokenKind kind, const char* start, size_t length, int int_value){
    Token t;
    t.kind = kind;
    t.start = start;
    t.length = length;
    t.int_value = int_value;
    t.line = lx->line;
    t.col = lx->col;
    return t;
}

static TokenKind keyword_or_ident(const char* start, size_t len){
 if( len == 3 && memcmp(start, "int", 3) == 0) return TOK_INT;
 if( len == 6 && memcmp(start, "return", 6) == 0) return TOK_RETURN;
 return TOK_IDENT;
}

void lexer_init(Lexer* lx, const char* src, size_t len){
    lx->src = src;
    lx->len = len;
    lx->pos = 0;
    lx->line = 1;
    lx->col = 1;
}


Token lexer_next(Lexer* lx){
    skip_ws(lx);


    const char* start = lx->src + lx->pos;
    char c = peek(lx);

    if (c == '\0'){
        return make_tok(lx, TOK_EOF, start, 0,0);
    }

    switch (c)
    {
    case '(':advance(lx); return make_tok(lx, TOK_LPAREN, start, 1,0);
    case ')': advance(lx); return make_tok(lx, TOK_RPAREN, start, 1, 0);
    case '{': advance(lx); return make_tok(lx, TOK_LBRACE, start, 1, 0);
    case '}': advance(lx); return make_tok(lx, TOK_RBRACE, start, 1, 0);
    case ';': advance(lx); return make_tok(lx, TOK_SEMI, start, 1, 0);
    case '+': advance(lx); return make_tok(lx, TOK_PLUS, start, 1, 0);
    case '-': advance(lx); return make_tok(lx, TOK_MINUS, start, 1, 0);
    case '*': advance(lx); return make_tok(lx, TOK_STAR, start, 1, 0);
    case '/': advance(lx); return make_tok(lx, TOK_SLASH, start, 1, 0);
    default:
        break;
    }


    
}