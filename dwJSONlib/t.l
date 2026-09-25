%{
#include "y.tab.h"
#include <stdlib.h>
#include <string.h>
#include "json_type.h"

MemoryPosition1 src_buf;

#define YY_INPUT(buf,result,max_size) \
		{ \
		int c = '*', n=0; \
		for ( ; n < max_size && \
			     src_buf.curr < src_buf.end  && \
				 (c =*src_buf.curr++) != '\n';  ++n ) \
			buf[n] = (char) c; \
		if ( c == '\n' ) \
			buf[n++] = (char) c; \
		result = n; \
		} \



%}

%%
[ \t\n\r]+          ; /* 공백 무시 */
"{"                 return TOKEN_LBRACE;
"}"                 return TOKEN_RBRACE;
"["                 return TOKEN_LBRACKET;
"]"                 return TOKEN_RBRACKET;
":"                 return TOKEN_COLON;
","                 return TOKEN_COMMA;
"true"              return TOKEN_TRUE;
"false"             return TOKEN_FALSE;
"null"              return TOKEN_NULL;

\"([^\"\\]|\\.)*\"  { 
					yylval.sval = new_strcopy(yytext, yyleng); 
					return TOKEN_STRING; 
					}
-?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)? {
					yylval.fval = atof(yytext);
					return TOKEN_NUMBER;
					}

.                   { 
					fprintf(stderr, "Unknown token: %s\n", yytext);
					exit(1); 
					}
%%

int yywrap() { return 1; }

