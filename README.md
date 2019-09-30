# A2

Name: Shreif Abdallah
NetID: selsaid
Partner Name : Meiwen Zhou
NetID: mzhou26

What we have in the folder: 
1) README
2) Makefile
3) parse.cpp
4) scan.cpp
5) scan.h
6) #parse.cpp#
7) input.txt (a tester with valid program ->  will print tree)
8) input2.txt (a tester file with invalid program -> will NOT print tree)
*compile instructions*:

	while in directory 

	$ make

*to modify tester file*:

	please modify input.txt to include the desired program

*to run our program*:
	
	$ make run
*to test*:
	$ make test-correct
	//that tests the correct program without errors
	$ make test-error
	//that tests the error program

	

*implementation details*:

	We started out by translating the provided parser from C to C++.
	We extended the language with if and while statements, relational operators rela_op() and condition production cond() in parse.cpp file.We modified scan.cpp file in order to be able to scan all the additional tokens that are added to the grammar.
	We implemented the error recovery mechanism of Niklaus Wirth from companion site. We referred to FIRST, FOLLOW, PREDICT sets and coded out the tokens for each set of every production statement in the grammar. Then we implemented the pseudo code from companion site by checking the input_token, if it doesn't match, the program will report whether "found in follow sets, keep excuting, i.e. insterted nonterminal x" or "deleted tokens found matching token in x" to indicate two different error_recovery situations. 
	We implemented AST structure by constructing a class called "treeNode" which has (1)string value: which will record the actual value on terminal nodes (2) list<treeNode> nodes: which will contain the list of children one treeNode has. We then converted stmt(), stmt_list(), cond(), expr(), term(), term_tail(), factor(), factor_tail(), rela_op(), add_op(),and mul_op() void functions to functions that return a pointer to the treeNode. In these functions, stmt_list(),term_tail(), and factor_tail() functions take a treeNode pointer as parameter because they can possibly go to epsilon transitions, in that case we should only return the treeNode that has been passed into the function. When building the printing function, we have to distinguish stmt_list from other productions because "[" "]" are used to wrap an arbitraty number of elements. The parentheses delimit tuples, which ha ve a fixed number of fields. 

*negative aspectives* : 
	1)Due to the limitation of phrase-error-recovery function, this program will not be able to identify all kinds of possible errors and skip them 
	2)No type checking, no static checks for uses of unintialized variables (program aborted)

*positive aspectives* :
	1)It works perfectly fine on recognizing valid program, and will generate a linear tree representation
	2)It works perfectly fine on checking errors. It will prompt the user there is an error, keep examing, until it finds another piece of readable code. 

*sample out put* :

INPUT: 
   read n
   cp := 2
   while n < 0
       found := 0
       cf1 := 2
       cf1s := cf1 * cf1
       while cf1s > cp
           cf2 := 2
           pr := cf1 * cf2
           while pr <= cp
               if pr == cp
                   found := 1
               end
               cf2 := cf2 + 1
               pr := cf1 * cf2
           end
           cf1 := cf1 + 1
           cf1s := cf1 * cf1
       end
       if found == 0
           write cp
           n := n - 1
       end
       cp := cp + 1
   end


OUTPUT:
	predict program --> stmt_list eof
predict stmt_list --> stmt stmt_list
predict stmt --> read id
matched read
matched id
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> while c sl end
matched while
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict rela_op --> st
matched st
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> mul_op factor factor_tail
predict mul_op --> mul
matched mul
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> while c sl end
matched while
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict rela_op --> gt
matched gt
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> mul_op factor factor_tail
predict mul_op --> mul
matched mul
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> while c sl end
matched while
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict rela_op --> stq
matched stq
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> if c sl end
matched if
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict rela_op --> eqeq
matched eqeq
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> epsilon
matched end
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> add_op term term_tail
predict add_op --> add
matched add
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> mul_op factor factor_tail
predict mul_op --> mul
matched mul
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> epsilon
matched end
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> add_op term term_tail
predict add_op --> add
matched add
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> mul_op factor factor_tail
predict mul_op --> mul
matched mul
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> epsilon
matched end
predict stmt_list --> stmt stmt_list
predict stmt --> if c sl end
matched if
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict rela_op --> eqeq
matched eqeq
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> write expr
matched write
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> add_op term term_tail
predict add_op --> sub
matched sub
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> epsilon
matched end
predict stmt_list --> stmt stmt_list
predict stmt --> id gets expr
matched id
matched gets
predict expr --> term term_tail
predict term --> factor factor_tail
predict factor --> id
matched id
predict factor_tail --> epsilon
predict term_tail --> add_op term term_tail
predict add_op --> add
matched add
predict term --> factor factor_tail
predict factor --> literal
matched literal
predict factor_tail --> epsilon
predict term_tail --> epsilon
predict stmt_list --> epsilon
matched end
predict stmt_list --> epsilon
matched eof
(program[(( read)( id "n"))( :=( id "cp")( num "2"))(( while)( <( id "n")( num "0"))[( :=( id "found")( num "0"))( :=( id "cf1")( num "2"))( :=( id "cf1s")( *( id "cf1")( id "cf1")))(( while)( cf1s( id "cf1s")( id "cp"))[( :=( id "cf2")( num "2"))( :=( id "pr")( *( id "cf1")( id "cf2")))(( while)( <=( id "pr")( id "cp"))[(( if)( ==( id "pr")( id "cp"))[( :=( id "found")( num "1"))]( end))( :=( id "cf2")( +( id "cf2")( num "1")))( :=( id "pr")( *( id "cf1")( id "cf2")))]( end))( :=( id "cf1")( +( id "cf1")( num "1")))( :=( id "cf1s")( *( id "cf1")( id "cf1")))]( end))(( if)( ==( id "found")( num "0"))[(( write)( id "cp"))( :=( id "n")( -( id "n")( num "1")))]( end))( :=( id "cp")( +( id "cp")( num "1")))]( end))])


*Thank you for reading.*


	




