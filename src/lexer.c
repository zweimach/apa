#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

Lexer* lexer_new(Char const* const input)
{
    Lexer* lexer = calloc(1, sizeof(Lexer));
    if (lexer) {
        lexer->input = calloc(strlen(input) + 1, sizeof(Char));
    }
    if (lexer->input && input) {
        strcpy(lexer->input, input);
    }
    return lexer;
}

Void lexer_free(Lexer* lexer)
{
    for (size_t i = 0; i < lexer->token_count; i++) {
        token_free(lexer->token_list[i]);
    }
    free(lexer->token_list);
    free(lexer->input);
    free(lexer);
}

Void lexer_read_char(Lexer* lexer)
{
    if (lexer->read_position >= strlen(lexer->input)) {
        lexer->ch = 0;
    } else {
        lexer->ch = lexer->input[lexer->read_position];
    }
    lexer->position = lexer->read_position;
    lexer->read_position += 1;
}

Void lexer_add_token(Lexer* lexer, Token* token)
{
    Token** token_list =
        realloc(lexer->token_list, sizeof(Token*) * (lexer->token_count + 1));
    if (token_list) {
        lexer->token_list = token_list;
        lexer->token_list[lexer->token_count] = token;
        lexer->token_count += 1;
    }
}

#define TERM_STR(x) ((Char[]){x, 0})

Token* lexer_next_token(Lexer* lexer)
{
    Token* token = NULL;
    lexer_read_char(lexer);
    switch (lexer->ch) {
    case '=':
        token = token_new(TOKEN_ASSIGN, TERM_STR(lexer->ch));
        break;
    case ';':
        token = token_new(TOKEN_SEMICOLON, TERM_STR(lexer->ch));
        break;
    case '(':
        token = token_new(TOKEN_OPEN_PAREN, TERM_STR(lexer->ch));
        break;
    case ')':
        token = token_new(TOKEN_CLOSE_PAREN, TERM_STR(lexer->ch));
        break;
    case ',':
        token = token_new(TOKEN_COMMA, TERM_STR(lexer->ch));
        break;
    case '+':
        token = token_new(TOKEN_PLUS, TERM_STR(lexer->ch));
        break;
    case '{':
        token = token_new(TOKEN_OPEN_CURLY, TERM_STR(lexer->ch));
        break;
    case '}':
        token = token_new(TOKEN_CLOSE_CURLY, TERM_STR(lexer->ch));
        break;
    case 0:
        token = token_new(TOKEN_EOF, TERM_STR('\0'));
        break;
    }
    if (token) {
        lexer_add_token(lexer, token);
    }
    return token;
}
