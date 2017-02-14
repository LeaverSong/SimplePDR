%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Parser}
%define parser_class_name {VMT_Parser}

%code requires{
    namespace Parser {
    class VMT_Driver;
    class VMT_Scanner;
    class ast_node;
    typedef ast_node * ast;
    }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#   if defined __cplusplus && 201103L <= __cplusplus
#       define YY_NULLPTR nullptr
#   else
#       define YY_NULLPTR 0
#   endif
# endif

}

%parse-param { VMT_Scanner  &scanner  }
%parse-param { VMT_Driver  &driver  }

%code{

#include <iostream>
#include <cstdlib>
#include <fstream>
   
/* include for all driver functions */
#include <parser/parser.h>

#undef yylex
#define yylex scanner.yylex

}

%define api.value.type variant
%define parse.assert

/* declare tokens */
%token          		OP CP
%token          		DECLARE_FUN DEFINE_FUN DECLARE_SORT DEFINE_SORT ASSERT
%token          		SET_LOGIC SET_OPTION
%token          		_TRUE _FALSE
%token          		_BOOLEAN _INTEGER
%token          		NEXT INIT INVAR TRANS
%token          		ANNOTATION
%token <std::string>   	IDENTIFIER DEFINE
%token          		AND OR NOT EQUAL
%token          		INDEX
%token          		COMMENT

%type<std::string> type
%type<ast> simple_expr expr
%type<std::vector<ast>> operands

%locations


%%

file            : OP statement CP 
                | file OP statement CP
                ;

statement       : DECLARE_FUN declaration
                | DEFINE_FUN definition
                | DECLARE_SORT declaration
                | DEFINE_SORT definition
                | ASSERT _TRUE
                ;

declaration     : IDENTIFIER OP CP type 
                ;

definition      : DEFINE OP CP type expr                  
                ;

type            : _BOOLEAN {$$ = "Bool";}
                | _INTEGER {$$ = "Int";}
                ;

expr            : OP ANNOTATION simple_expr annotation CP 
				  { $$ = $3; }
                | simple_expr { $$ = $1; }
                ;

annotation		:
				| NEXT simple_expr annotation 
				| INIT _TRUE annotation
				| INVAR INDEX annotation
				| TRANS _TRUE annotation
				;
				

simple_expr     : IDENTIFIER           { $$ = driver.mk_var($1); }
				| DEFINE			   { $$ = driver.add_ast($1);}
                | OP NOT operands CP   { $$ = driver.mk_not($3); }
                | OP AND operands CP   { $$ = driver.mk_and($3); }
                | OP OR operands CP    { $$ = driver.mk_or($3); }
                | OP EQUAL operands CP { $$ = driver.mk_eq($3); }
                | _TRUE 
                | _FALSE
                ;

operands		: simple_expr operands { $$.push_back($1); }
				| simple_expr { $$.push_back($1); }
				;

%%


void Parser::VMT_Parser::error(const location_type &l, 
                          const std::string &err_message) {

    std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
