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
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof","if","while",
                        "end","eqeq","neq","gt","st","gtq","stq"};

static token input_token;
static int tokenNumber = 0; //Index of token reading in
static token p_follow[] = {};
static token sl_follow[]= 
static token s_follow[] = {t_id,t_read,t_write,t_if,t_while,t_eof};
static token c_follow[] = {t_id,t_read,t_write,t_if,t_while,t_eof,t_end};
static token e_follow[] = {t_id,t_read,t_write,t_if,t_while,t_eof,t_end,t_rparen,t_}

void program ();
void stmt_list ();
void stmt ();
void expr ();
void cond();
void term_tail ();
void term ();
void factor_tail ();
void factor ();
void add_op ();
void mul_op ();
void rela_op();
void match();

/*
//Functions to help print the syntax trees including preIndent, postIndent, and prefix
string postIndent(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str += " ";
  }
  return str;
}

string preIndent(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str = " " + str;
  }
  return str;
}

string prefix(string str, string tail){
  if(tail == "") return str;
  for (int i = 0; i < tail.length(); ++i){
    if(tail[i] == ' '){
      return tail.substr(0,i)+" "+ str +" "+ tail.substr(i+1, tail.length() - i);
    }
  }
  return "prefix error";
}
//check if t is in the set[]
int contains(token t, token set[]){
  int i = 0;
  while(set[i]){
    if (t == set[i++]) {
      return 1;
    }
  }
  return 0;
}

*/
void error () {

    cout << "Error detected: "++ names[input_token];
    exit(1)
}

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        input_token = scan();
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
            cout << "predict stmt --> if expr\n";
            match(t_if);
            // TODO: need to initiate C prouction here
            //c();
            stmt_list();
            match(t_eof);
            break;
        case t_while:
            cout << "predict stmt --> while expr\n";
            match(t_while);
           // c();
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

void cond (){
    Node* lhs;
    Node* rhs;

    switch(input_token){
        case t_id:
        case t_literal:
            lhs = expr();
            ro = rela_op();
            rhs = expr();
        case t_lparen:
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
            cout << ("predict mul_op --> mul\n");
            match (t_mul);
            break;
        case t_div:
            cout << ("predict mul_op --> div\n");
            match (t_div);
            break;
        default: error ();
    }
}

void rela_op() {
    switch (input_token){
        case t_eqeq:
            cout << ("predict rela_op --> eqeq\n");
            match (t_eqeq);
            break;
        case t_neq:
            cout << ("predict rela_op --> neq\n");
            match (t_neq);
            break;
        case t_gt:
            cout << ("predict rela_op --> gt\n");
            match (t_gt);
            break;
        case t_st:
            cout << ("predict rela_op --> st\n");
            match (t_st);
            break;
       case t_gtq:
            cout << ("predict rela_op --> gta\n");
            match (t_gtq);
            break;   
       case t_stq:
            cout << ("predict rela_op --> stq\n");
            match (t_stq);
            break;     
    }
}



int main () {
    input_token = scan ();
  
}