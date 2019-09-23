#include<iostream>  
#include <string.h>

#include "scan.h"

using namespace std;

char token_image[MAX_TOKEN_LEN];

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        c = getchar();
    }
    if (c == EOF)
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            if (i >= MAX_TOKEN_LEN) {
                cout << ("max token length exceeded\n");
                exit(1);
            }
            c = getchar();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "while")) return t_write;
        else if (!strcmp(token_image, "if")) return t_write;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                cout << stderr << "error\n";
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
        case '<': 
            if((c = getchar()) == '='){
                c = getchar();
                return t_stq;
            }
            else {
                c = getchar();
                return t_st;
            };
        case '>': 
            if((c = getchar()) == '='){
                c = getchar();
                return t_gtq;
            }
            else {
                c = getchar();
                return t_gt;
            }; 
        case '=': 
            if((c = getchar()) == '='){
                c = getchar();
                return t_eqeq;
            }
            else {
                cout << stderr << "error\n";
                exit(1);
            };
        case '!': 
            if((c = getchar()) == '='){
                c = getchar();
                return t_neq;
            }
            else {
                cout << stderr << "error\n";
                exit(1);
            };                               
        default:
            cout << ("error\n");
            exit(1);
    }
}
