#include <iostream>  
#include <cstdlib>
#include <iterator> 
#include <string>
#include "scan.h"
#include <set>
#include <map>
#include <list>

using namespace std;

/*
Dictionary: 
SL - stmt_list
S -stmt
*/


/* construct treeNode struct to build AST tree */
class treeNode{
    public:

    string value;
    list<treeNode> nodes;
    treeNode(string v, list<treeNode> n){
        value = v;
        nodes = n;
    } 
};

const string  names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof","if","while",
                        "end","eqeq","neq","gt","st","gtq","stq"};

static token input_token;
static bool isError;
static map<string,set<token> > first;
static map<string,set<token> > follow;
static set<string> eps;
static list<treeNode> emptyL;
static treeNode * root = new treeNode("program",emptyL);
static treeNode * emptyN = new treeNode("",emptyL);

static bool isValidProgram = true;
void generate_eps(){
    eps = {"SL","TT","FT"};
}

void generate_first();
void generate_follow();
void program ();
treeNode* stmt_list (treeNode* input_node);
treeNode* stmt ();
treeNode* expr ();
treeNode* cond();
treeNode* term_tail (treeNode* input_node);
treeNode* term ();
treeNode* factor_tail (treeNode* input_node);
treeNode* factor ();
treeNode* add_op ();
treeNode* mul_op ();
treeNode* rela_op();
string match(token expected);
void error_recovery(string str);

void generate_first(){


     first.insert(pair<string, set<token>>("P", {t_id, t_read, t_write, t_if, t_while, t_eof}));
     first.insert(pair<string, set<token>>("SL", {t_id, t_read, t_write, t_if, t_while}));
     first.insert(pair<string, set<token>>("S", {t_id, t_read, t_write, t_if, t_while}));
     first.insert(pair<string, set<token>>("C", {t_lparen, t_id, t_literal}));
     first.insert(pair<string, set<token>>("E", {t_lparen, t_id, t_literal}));
     first.insert(pair<string, set<token>>("T", {t_lparen, t_id, t_literal}));
     first.insert(pair<string, set<token>>("F", {t_lparen, t_id, t_literal}));
     first.insert(pair<string, set<token>>("TT", {t_add, t_sub}));
     first.insert(pair<string, set<token>>("FT", {t_mul, t_div}));
     first.insert(pair<string, set<token>>("ro", {t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq}));
     first.insert(pair<string, set<token>>("ao", {t_add, t_sub}));
     first.insert(pair<string, set<token>>("mo", {t_mul, t_div}));
}

void generate_follow(){
    follow.insert(pair<string, set<token>>("P", {}));
     follow.insert(pair<string, set<token>>("SL", {t_end, t_eof}));
     follow.insert(pair<string, set<token>>("S", {t_id, t_read, t_write, t_if, t_while, t_eof, t_end}));
     follow.insert(pair<string, set<token>>("C", {t_id, t_read, t_write, t_if, t_while, t_end}));
     follow.insert(pair<string, set<token>>("E", {t_id, t_read, t_write, t_if, t_while, t_eof, t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq, t_end, t_rparen}));
     follow.insert(pair<string, set<token>>("T", {t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq, t_end, t_rparen}));
     follow.insert(pair<string, set<token>>("F", {t_mul, t_div, t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq, t_end, t_rparen}));
     follow.insert(pair<string, set<token>>("TT", {t_id, t_read, t_write, t_if, t_while, t_eof, t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq, t_end, t_rparen}));
     follow.insert(pair<string, set<token>>("FT", {t_add, t_sub, t_id, t_read, t_write, t_if, t_while, t_eof, t_eqeq, t_neq, t_gt, t_st, t_gtq, t_stq, t_end, t_rparen}));
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
void error (string statement) {
	cout << "found in follow sets, keep excuting, i.e.inserted nonterminal " +statement << "\n";  
//    cout << "Error detected: "<< names[input_token] << "\n";
    return;
}

string match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token] << "\n";
        
        //treeNode node = new treeNode(value, nodes);
	string value = "";
        if (input_token == t_id){
	  value.append("id ");
          value.append("\"");
          value.append(token_image);
          value.append("\"");
        }else if(input_token == t_literal){
	 value.append("num ");
	 value.append("\"");
	 value.append(token_image);
	 value.append( "\""); 
	}
	else{
	//value.append(names[input_token]);
	value.append(token_image);
	}
	value = " "+value;
        input_token = scan();
        return value;
    }else if(input_token == t_eof){
        return "";
    }
    else{
        cout << "token not matched. \n";
        throw "match error";
    };
}

void program () {
    error_recovery("P");
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:{
            if(isError){
                cout << "deleted tokens found matching token in P \n";
            }   
            cout << "predict program --> stmt_list eof\n";
	    list<treeNode> list;
	    treeNode * node = new treeNode("8u",list);
            root->nodes.push_back((*stmt_list(node)));
           //stmt_list ();
            treeNode endNode = * new treeNode(match(t_eof),emptyL);
            //root->nodes.push_back(endNode);
            //match (t_eof);
            break;
        }
    // MEIWEN: adding if and while conditions
        default: error ("P");
    }
}


/* tree printing function */
string printTree (treeNode * root){
	if((root->nodes).empty()){
	    return "(" + root->value + ")";
	}
	
	string str="";
	if(root->value.compare( "8u")!=0){
	 str = "(" + root->value;
	}	
	else{
	str = "[";
	}
	for(treeNode child : root-> nodes){
		str = str + printTree(&child);
	}
        if(root->value.compare( "8u")!=0){
        str = str + ")";

        }else{ 
        str = str + "]";
        }

	return str;
}


treeNode* stmt_list (treeNode* input_node) {
    error_recovery("SL");
    switch (input_token) {
        case t_id:
        case t_read:
	case t_if:
	case t_while:
        case t_write:{
                cout << "predict stmt_list --> stmt stmt_list\n";

            input_node->nodes.push_back((*stmt()));
           //(*stmt_list(input_node)));
            //stmt ();
            //stmt_list ();
            //break;
	    return stmt_list(input_node);
            }
	case t_end:
        case t_eof:
            cout << "predict stmt_list --> epsilon\n";
            return input_node;         /*  epsilon production */
        default: {
            error ("SL");
            return input_node;
        }
    }
}

treeNode* stmt () {
    error_recovery("S");
    switch (input_token) {
        case t_id:{
            if(isError){
                cout << "deleted tokens found matching token in S \n";
            } 
            cout << "predict stmt --> id gets expr\n";
    
            treeNode * idNode = new treeNode(match(t_id),emptyL);
            treeNode * getsNode = new treeNode(match(t_gets),emptyL);
           // treeNode * node = new treeNode("",emptyL);
            getsNode->nodes.push_front(*idNode);
            getsNode->nodes.push_back(*expr());
            //match (t_id);
            //match (t_gets);
            //expr ();
            return getsNode;
        }
        case t_read:{
             if(isError){
                cout << "deleted tokens found matching token in S \n";
            } 
            cout << "predict stmt --> read id\n";
            treeNode * readNode = new treeNode(match(t_read),emptyL);
            treeNode * idNode = new treeNode(match(t_id),emptyL);
            treeNode * node = new treeNode("",emptyL);
            node->nodes.push_front(*readNode);
            node->nodes.push_back(*idNode);
            // match (t_read);
            // match (t_id);
            // break;
            return node;
        }
        case t_write:{
            if(isError){
                cout << "deleted tokens found matching token in S \n";
            } 
            cout << "predict stmt --> write expr\n";

            treeNode * node = new treeNode("",emptyL);
            treeNode * writeNode = new treeNode(match(t_write),emptyL);
            node->nodes.push_front(*writeNode);
            node->nodes.push_back((*expr()));
            // match (t_write);
            // expr ();
            // break;
            return node;
        }
        case t_if:{
	        if(isError){
                cout << "deleted tokens found matching token in S \n";
            } 
            cout << "predict stmt --> if c sl end\n";
	    
	    list<treeNode> l;
	    list<treeNode> l2;
	    treeNode * node = new treeNode("",l);
            treeNode * SLnode = new treeNode("8u",l2);
            treeNode * ifNode = new treeNode(match(t_if),emptyL);
            node->nodes.push_front(*ifNode);
            node->nodes.push_back((*cond ()));
            node->nodes.push_back((*stmt_list(SLnode)));
            treeNode * endNode = new treeNode(match(t_end),emptyL);
            node->nodes.push_back(*endNode);
            //match(t_if);
            //cond ();
            //stmt_list();
            //match( ";t_end);
            return node;}
        case t_while:{
           if(isError){
                cout << "deleted tokens until found matching token in S \n";
            } 
            cout << "predict stmt --> while c sl end\n";
	    
	    list<treeNode> l;
	    list<treeNode> l2;
	    treeNode * node = new treeNode("",l2);
            treeNode * SLnode = new treeNode("8u",l);
            treeNode * whileNode = new treeNode(match(t_while),emptyL);
            node->nodes.push_front(*whileNode);
            node->nodes.push_back((*cond ()));
            node->nodes.push_back((*stmt_list(SLnode)));
            treeNode * endNode = new treeNode(match(t_end),emptyL);
            node->nodes.push_back(*endNode);

            //match(t_while);
            //cond();
            //stmt_list();
            //match(t_eof);
            return node;     
        }
        default:{
           error ("S");
           return emptyN;     
        } 
    }
}

treeNode* expr () {
    error_recovery("E");
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:{
            if(isError){
                cout << "deleted found matching token in E \n";
            }
            cout << "predict expr --> term term_tail\n";
            // term ();
            // term_tail ();
            // break;
            return term_tail(term());
        }
        default: {
            error ("E");
            return emptyN;
        }
    }
}

treeNode* cond (){
    error_recovery("C");

    switch(input_token){
        case t_id:
        case t_literal:
        case t_lparen:{
            if(isError){
                cout << "deleted found matching token in C \n";
            }

            treeNode expNode = ((*expr()));
            treeNode * node = rela_op();
	    node->nodes.push_front(expNode);
            node->nodes.push_back((*expr()));
            // expr();
            // rela_op();
            // expr();
            // break;
            return node;
        }
        default : {
            error ("C");
            return emptyN;
        }
    }
}

treeNode* term_tail (treeNode* input_node) {
    error_recovery("TT");
    switch (input_token) {
        case t_add:
        case t_sub:{
            cout << "predict term_tail --> add_op term term_tail\n";
            treeNode * node = add_op();
            node->nodes.push_front(*input_node);
            node->nodes.push_back((*term_tail(term())));
            return node;
            // add_op ();
            // term ();
            // term_tail ();
            // break;
	}
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eqeq:
        case t_gt:
        case t_st:
        case t_neq:
        case t_gtq:
        case t_stq:
        case t_end:
        case t_eof:{
            if(isError){
                cout << "deleted found matching token in TT \n";
             }
            cout << "predict term_tail --> epsilon\n";
            return input_node;         /*  epsilon production */
        }
        default:{
             error ("TT");
             return emptyN;
            } 
    }
}

treeNode* term () {
    error_recovery("T");
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:{  
            if(isError){
                cout << "deleted found matching token in T \n";
            }
            cout << "predict term --> factor factor_tail\n";
            // factor ();
            // factor_tail ();
            // break;}
            return factor_tail(factor());
	}
        default: {
            error ("T");
            return emptyN;
        }
    }
}

treeNode* factor () {
    error_recovery("F");
    switch (input_token) {
        case t_id :{
            if(isError){
                cout << "deleted found matching token in FT \n";
            }
            
            cout << ("predict factor --> id\n");
            treeNode * node = new treeNode(match(t_id), emptyL);
            return node;
        } 
        case t_literal: {
            if(isError){
                cout << "deleted found matching token in FT \n";
            }

            cout << ("predict factor --> literal\n");
            treeNode * node = new treeNode(match(t_literal), emptyL);
            return node;
        }

        case t_lparen:{
            if(isError){
                cout << "deleted found matching token in FT \n";
                }

            cout << ("predict factor --> lparen expr rparen\n");

            treeNode * nodeFront = new treeNode(match(t_lparen),emptyL);
            treeNode * nodeEnd = new treeNode(match(t_rparen), emptyL);
            list<treeNode> nodeList;
            nodeList.push_front(*nodeFront);
            nodeList.push_back(*nodeEnd);
            treeNode * node = new treeNode("",nodeList);
            
            return node;
            } 
        default:{
            error ("F");
            return emptyN;
        }
    }
}

treeNode* factor_tail (treeNode* input_node) {
    error_recovery("FT");
    switch (input_token) {
        case t_mul:
        case t_div:{
            printf ("predict factor_tail --> mul_op factor factor_tail\n");
            
            treeNode * node = mul_op();
            node->nodes.push_front(*input_node);
            node->nodes.push_back((*factor_tail(factor())));
          
            return node;
        }
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_eqeq:
        case t_gt:
        case t_st:
        case t_neq:
        case t_gtq:
        case t_stq:
        case t_while:
        case t_if:
        case t_end:
        case t_write:
        case t_eof:
            if(isError){
                cout << "deleted found matching token in FT \n";
            }

            cout << "predict factor_tail --> epsilon\n";
            return input_node;        /*  epsilon production */
        default:{
            error ("FT");
            return emptyN;
        } 
    }
}

treeNode* add_op () {
    error_recovery("ao");
    switch (input_token) {
        case t_add:{
        if(isError){
        cout << "deleted found matching token in ao \n";
        }
            cout << ("predict add_op --> add\n");
            treeNode * node = new treeNode(match(t_add), emptyL);
            return node;
	}
        case t_sub:{
        if(isError){
        cout << "deleted found matching token in ao \n";
        }
            cout << ("predict add_op --> sub\n");
            treeNode * node = new treeNode(match(t_sub), emptyL);
            return node;
	}
        default:{
            error ("ao"); 
            return emptyN;
        } 
    }
}

treeNode* mul_op () {
    error_recovery("mo");
    switch (input_token) {
        case t_mul:{
        if(isError){
        cout << "deleted found matching token in mo \n";
        }
            cout << ("predict mul_op --> mul\n");
            treeNode * node = new treeNode(match(t_mul), emptyL);
            return node;
	}
        case t_div:{
        if(isError){
        cout << "deleted found matching token in mo \n";
        }
            cout << ("predict mul_op --> div\n");
            treeNode * node = new treeNode(match(t_div), emptyL);
            return node;
	}
        default: {
            error ("mo");
            return emptyN;
        }
    }
}

treeNode* rela_op() {
    error_recovery("ro");
    switch (input_token){
        case t_eqeq:{
        if(isError){
        cout << "deleted found matching token in ro \n";
        }
            cout << ("predict rela_op --> eqeq\n");
            treeNode * node = new treeNode(match(t_eqeq), emptyL);
            return node;
        }
        case t_neq:{
        if(isError){
        cout << "deleted found matching token in ro \n";
        }

            cout << ("predict rela_op --> neq\n");
            treeNode * node = new treeNode(match(t_neq), emptyL);
            return node;
        }
        case t_gt:{
        if(isError){
        cout << "deleted found matching token in ro \n";
        }

            cout << ("predict rela_op --> gt\n");
            treeNode * node = new treeNode(match(t_gt), emptyL);
            return node;
        }
        case t_st:{
        if(isError){
        cout << "deleted found matching token in ro \n";
        }

            cout << ("predict rela_op --> st\n");
            treeNode * node = new treeNode(match(t_st), emptyL);
            return node;
        }
       case t_gtq:{
        if(isError){
                cout << "deleted found matching token in ro \n";
            }
            cout << ("predict rela_op --> gta\n");
            treeNode * node = new treeNode(match(t_gtq), emptyL);
            return node;
       }
       case t_stq:{
            if(isError){
                cout << "deleted found matching token in ro \n";
            }

            cout << ("predict rela_op --> stq\n");
            treeNode * node = new treeNode(match(t_stq), emptyL);
            return node;
       }   
        default:{
            error("ro");
            return emptyN;
            }; 
    }

}

void error_recovery(string statement){
    if(!(first.at(statement).count(input_token) || eps.count(statement))){
        cout << "error \n";
        while(!(first.at(statement).count(input_token) || follow.at(statement).count(input_token) || input_token == t_eof)) {
            cout << "delete token " + names[input_token] << "\n";
            input_token = scan();
	}
	isValidProgram = false;
	isError = true;
	return;
    }
	isError = false;
}

int main () {
     generate_first();
     generate_follow();
    generate_eps();
    input_token = scan ();
    program();

  if(isValidProgram)cout <<  printTree(root);
}
