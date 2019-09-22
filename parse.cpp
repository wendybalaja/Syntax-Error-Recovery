#include<iostream>  
#include <list> 
#include <iterator> 
#include <string>
#include "scan.h"
using namespace std;

/*
Dictionary: 
SL - stmt_list
S -stmt
*/
static token input_token;



const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof","if","while"};

void program ();
void stmt_list ();
void stmt ();
void expr ();
void term_tail ();
void term ();
void factor_tail ();
void factor ();
void add_op ();
void mul_op ();
void match();

void error () {
    cout << "Please provide the number of nodes: ";
    exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        if (input_token == t_id || input_token == t_literal)
            cout << ": " << token_image;

        cout << endl;
        input_token = scan ();
    }
    else error ();
}

void program () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
            cout << "predict program --> stmt_list eof\n";
            stmt_list ();
            match (t_eof);
            break;
    // MEIWEN: adding if and while conditions
        case t_if:
        case t_while:
        default: error ();
    }
}

void stmt_list () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
            cout << "predict stmt_list --> stmt stmt_list\n";
            stmt ();
            stmt_list ();
            break;
        case t_eof:
            cout << "predict stmt_list --> epsilon\n";
            break;          /*  epsilon production */
        default: error ();
    }
}

void stmt () {
    switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets expr\n";
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
            cout << "predict stmt --> read id\n";
            match (t_read);
            match (t_id);
            break;
        case t_write:
            cout << "predict stmt --> write expr\n";
            match (t_write);
            expr ();
            break;
        case t_if:
            count << "predict stmt --> if expr\n";
            match(t_if);
            // TODO: need to initiate C prouction here
            c();
            stmt_list();
            match(t_eof);
            break;
        case t_while:
            count << "predict stmt --> while expr\n"
            match(t_while);
            c();
            stmt_list();
            match(t_eof);
            break;
        default: error ();
    }
}

void expr () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail\n";
            term ();
            term_tail ();
            break;
        default: error ();
    }
}


void term_tail () {
    switch (input_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail\n";
            add_op ();
            term ();
            term_tail ();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
            cout << "predict term_tail --> epsilon\n";
            break;          /*  epsilon production */
        default: error ();
    }
}

void term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail\n";
            factor ();
            factor_tail ();
            break;
        default: error ();
    }
}

void factor_tail () {
    switch (input_token) {
        case t_mul:
        case t_div:
            printf ("predict factor_tail --> mul_op factor factor_tail\n");
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
             cout << "predict factor_tail --> epsilon\n";
            break;          /*  epsilon production */
        default: error ();
    }
}

void factor () {
    switch (input_token) {
        case t_id :
            cout << ("predict factor --> id\n");
            match (t_id);
            break;
        case t_literal:
            cout << ("predict factor --> literal\n");
            match (t_literal);
            break;
        case t_lparen:
            cout << ("predict factor --> lparen expr rparen\n");
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default: error ();
    }
}

void add_op () {
    switch (input_token) {
        case t_add:
            cout << ("predict add_op --> add\n");
            match (t_add);
            break;
        case t_sub:
            cout << ("predict add_op --> sub\n");
            match (t_sub);
            break;
        default: error ();
    }
}

void mul_op () {
    switch (input_token) {
        case t_mul:
            cout << ("predict mu_l_op --> mul\n");
            match (t_mul);
            break;
        case t_div:
            cout << ("predict mul_op --> div\n");
            match (t_div);
            break;
        default: error ();
    }
}




int main () {
    input_token = scan ();
  
}