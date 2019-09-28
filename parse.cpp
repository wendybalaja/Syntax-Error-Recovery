#include<iostream>  
#include <cstdlib>
#include <iterator> 
#include <string>
#include "scan.h"
#include <set>
#include <map>

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

static int tokenNumber = 0; //Index of token reading in


static map<string,set<token> > first;
static map<string,set<token> > follow;
static set<string> eps;

void generate_eps(){
    eps = {"SL","TT","FL"};
}

void generate_first();
void generate_follow();
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


void generate_first(){
    first.insert(pair<string, set<token> >("P", {}));
    first.insert(pair<string,set<token> >("SL",{t_id,t_read,t_write,t_if, t_while}));
    first.insert(pair<string,set<token> >("S",{t_id,t_read,t_write,t_if,t_while}));
    first.insert(pair<string,set<token> >("C",{t_lparen,t_id,t_lit}));
    first.insert(pair<string,set<token>>("E",{t_lparen,t_id,t_lit}));
    first.insert(pair<string,set<token>>("T",{t_lparen,t_id,t_lit}));
    first.insert(pair<string,set<token>>("F",{t_lparen,t_id,t_lit}));
    first.insert(pair<string,set<token>>("TT",{t_add,t_sub}));
    first.insert(pair<string,set<token>>("FT",{t_mul,t_div}));
    first.insert(pair<string,set<token>>("ro",{t_eqeq,t_neq,t_gt,t_st,t_gtq,t_stq}));
    first.insert(pair<string,set<token>>("ao",{t_add,t_sub}));
    first.insert(pair<string,set<token>>("mo",{t_mul,t_div}));
}

void generate_follow(){
     follow.insert(pair<string, set<token>>("P", {}));
     follow.insert(pair<string, set<token>>("SL", {t_end, t_eof}));
     follow.insert(pair<string, set<token>>("S", {t_id, t_read, t_write, t_if, t_while, t_eof, t_end}));
     follow.insert(pair<string, set<token>>("C", {t_id, t_read, t_write, t_if, t_while, t_end}));
     follow.insert(pair<string, set<token>>("E", {t_id, t_read, t_write, t_if, t_while, t_eof, t_eq, t_noteq, t_greater, t_less, t_eqorgreater, t_eqorless, t_end, t>
     follow.insert(pair<string, set<token>>("T", {t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eq, t_noteq, t_greater, t_less, t_eqorgreater, t_eqor>
     follow.insert(pair<string, set<token>>("F", {t_mul, t_div, t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eq, t_noteq, t_greater, t_less, t_eqorg>
     follow.insert(pair<string, set<token>>("TT", {t_id, t_read, t_write, t_if, t_while, t_eof, t_eq, t_noteq, t_greater, t_less, t_eqorgreater, t_eqorless, t_end, >
     follow.insert(pair<string, set<token>>("FT", {t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eq, t_noteq, t_greater, t_less, t_eqorgreater, t_eqo>
     follow.insert(pair<string, set<token>>("ro", {t_id, t_literal, t_lparen}));
     follow.insert(pair<string, set<token>>("ao", {t_id, t_literal, t_lparen}));
     follow.insert(pair<string, set<token>>("mo", {t_id, t_literal, t_lparen}));
};
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

    cout << "Error detected: "<< names[input_token] << "\n";
    return;
}

void match (token expected) {
    if (input_token == expected) {
        // cout << "matched " << names[input_token];
        input_token = scan();
        // if (input_token == t_id || input_token == t_literal)
        //     cout << ": " << token_image;

        // cout << endl;
        // input_token = scan ();
    }else if(input_token == t_eof){
        cout << "end of line matched. \n"
        return;
    }
    else{
        cout << "token not matched. \n"
        input_token = scan();
        match(expected);
    };
}

void program () {
    error_recovery("P");
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
    error_recovery("SL");
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
    error_recovery("S");
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
    error_recovery("E");
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
    error_recovery("C");

    switch(input_token){
        case t_id:
        case t_literal:
        case t_lparen:
            expr();
            rela_op();
            expr();
            break;
        default : error ();
    }
}

void term_tail () {
    error_recovery("TT");
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
    error_recovery("T");
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
    error_recovery("FT");
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
    error_recovery("F");
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
    error_recovery("ao");
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
    error_recovery("mo");
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
    error_recovery("ro");
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

void error_recovery(string statement){
    if(!(first.at(statement).counts(input_token) || eps.counts(statement))){
        cout << "error \n";
        do{
            cout << "delete token \n" + names[input_token];
            input_token = scan();

        }while(!(first.at(statement).counts(input_token) || follow.at(statement).counts(input_token) || input_token == t_eof));
    }
}

int main () {
     first.insert(pair<string, set<token> >("P", {t_eof,t_id,t_read,t_write,t_if,t_while}));
    // generate_first();
    // generate_follow();
    // generate_eps();
    input_token = scan ();
    program();
  
}