#include "lexer.h"
#include <ctype.h>
#include <string.h>

static char peek(Lexer* lx){
    if(lx->pos >= lx->len) return '\0';
    return lx->src[lx->pos];
}

static char peek_next(Lexer* lx){
    if(lx->pos + 1 >= lx->len) return '\0';
    return lx->src[lx->pos + 1];
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
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n'){
            advance(lx);
            continue;
        }

        if(c == '/' && peek_next(lx) == '/'){
            advance(lx);
            advance(lx);
            while(peek(lx) != '\0' && peek(lx) != '\n'){
                advance(lx);
            }
            continue;
        }

        if(c == '/' && peek_next(lx) == '*'){
            advance(lx);
            advance(lx);
            while(peek(lx) != '\0'){
                if(peek(lx) == '*' && peek_next(lx) == '/'){
                    advance(lx);
                    advance(lx);
                    break;
                }
                advance(lx);
            }
            continue;
        }

        break;
    }
}

static int is_ident_char(char c){
    return isalnum((unsigned char)c) || c == '_';
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


    if(isdigit((unsigned char)c)){
        int value = 0;
        size_t n = 0;
        while (isdigit((unsigned char)peek(lx)))
        {
            char d = advance(lx);
            value = value *  10 + (d - '0');
            n++;
        }
        if(is_ident_char(peek(lx))){
            while(is_ident_char(peek(lx))){
                advance(lx);
                n++;
            }
            return make_tok(lx, TOK_INVALID, start, n, 0);
        }
        return make_tok(lx, TOK_INT_LIT, start, n, value);
    }


    if(isalpha((unsigned char)c) || c == '_'){
        size_t n =0;
        while(is_ident_char(peek(lx))){
            advance(lx);
            n++;
        }

        TokenKind k = keyword_or_ident(start, n);
        return make_tok(lx,k,start,n,0);
    }

    advance(lx);
    return make_tok(lx, TOK_INVALID, start, 1, 0);
}


const char* token_kind_str(TokenKind k){
    switch (k)
    {
    case TOK_INT: return "TOK_INT";
    case TOK_RETURN: return "TOK_RETURN";
    case TOK_IDENT: return "TOK_IDENT";
    case TOK_INT_LIT: return "TOK_INT_LIT";
    case TOK_LPAREN: return "TOK_LPAREN";
    case TOK_RPAREN: return "TOK_RPAREN";
    case TOK_LBRACE: return "TOK_LBRACE";
    case TOK_RBRACE: return "TOK_RBRACE";
    case TOK_SEMI: return "TOK_SEMI";
    case TOK_PLUS: return "TOK_PLUS";
    case TOK_MINUS: return "TOK_MINUS";
    case TOK_STAR: return "TOK_STAR";
    case TOK_SLASH: return "TOK_SLASH";
    case TOK_EOF: return "TOK_EOF";
    case TOK_INVALID: return "TOK_INVALID";
    default:
        return "TOK_???";
    }
}
