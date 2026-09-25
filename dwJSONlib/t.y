%{

#include <stdio.h>
#include <stdlib.h>
#include "json_type.h"
int yylex();
void yyerror(const char *s);


struct Object_t* gJson = 0;


%}

%union {
    const char *sval;
    double fval;
    
    struct Object_t* obj;
    struct Array_t* arr;
    struct Pair_t* pair;
    struct Value_t* val;
}

%token TOKEN_LBRACE TOKEN_RBRACE TOKEN_LBRACKET TOKEN_RBRACKET TOKEN_COLON TOKEN_COMMA
%token TOKEN_TRUE TOKEN_FALSE TOKEN_NULL
%token TOKEN_OBJECT TOKEN_ARRAY
%token <sval> TOKEN_STRING TOKEN_NUMBER
%type <obj> json
%type <obj> object
%type <arr> array
%type <val> value elements
%type <pair> pair members



%%

json:
      object { gJson = $1; }
;
value:
      object        { 
                    Value_t * v = new_Value_t(TOKEN_OBJECT);
                    v->obj = $1;
                    $$ = v; 
                    }
    | array         { 
                    Value_t * v = new_Value_t(TOKEN_ARRAY);
                    v->arr = $1;
                    $$ = v; 
                    }
    | TOKEN_STRING { 
                    Value_t * v = new_Value_t(TOKEN_STRING);
                    v->sval = yylval.sval;
                    $$ = v; 
                    }
    | TOKEN_NUMBER { 
                    Value_t * v = new_Value_t(TOKEN_NUMBER);
                    v->fval = yylval.fval;
                    $$ = v; 
                    }
    | TOKEN_TRUE    { 
                    Value_t * v = new_Value_t(TOKEN_TRUE);
                    v->sval = "true";
                    $$ = v; 
                    }
    | TOKEN_FALSE   { 
                    Value_t * v = new_Value_t(TOKEN_FALSE);
                    v->sval = "false";
                    $$ = v; 
                    }
    | TOKEN_NULL    { 
                    Value_t * v = new_Value_t(TOKEN_NULL);
                    v->sval = "null";
                    $$ = v; 
                    }
;
object:
      TOKEN_LBRACE TOKEN_RBRACE {
            Object_t* o = new_Object_t();
            o->start = 0;
            $$ = o;
      }
    | TOKEN_LBRACE members TOKEN_RBRACE {
            Object_t* o = new_Object_t();
            o->start = $2;
            $$ = o;
    }
;
members:
      pair { $$ = $1; }
    | members TOKEN_COMMA pair {
            Pair_t* v = $1;
            while(v->next) v = v->next;
            v->next = $3;
            $$ = $1;
    }
;
pair:
      TOKEN_STRING TOKEN_COLON value {
                    Pair_t* v = new_Pair_t($1,$3 );
                    $$ = v;
                    }
;
array:
      TOKEN_LBRACKET TOKEN_RBRACKET {
            Array_t* arr = new_Array_t();
            arr->start = 0;
            $$ = arr;
      }
    | TOKEN_LBRACKET elements TOKEN_RBRACKET {
            Array_t* arr = new_Array_t();
            arr->start = $2;
            $$ = arr;            
    }
;
elements:
      value { $$=$1;}
    | elements TOKEN_COMMA value {
            Value_t* v = $1;
            while(v->next) v = v->next;
            v->next = $3;
            $$ = $1;
    }
;    

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

extern FILE *yyin; 
extern MemoryPosition1 src_buf;

/*
int main1() {

    Load_File(&src_buf, "t.json");

    yyparse();
    printf("Valid JSON!\n");


    if(gJson) Print_Json(stdout, gJson);
    
    printf("\n~~~~~~~~~~~~~~\n");

    // FreeMemoryPosition1(&src_buf);
    return 0;
}
*/

    // FILE* fp;
    // fopen_s(&fp, "t.json", "r");
    // yyin = fp;
    //if( fclose(fp) == 0)
    //    printf("fclose !!\n");

