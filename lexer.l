%option noyywrap
%option yylineno
%{
#include <string>
#include <iostream>
#include "parser.tab.hpp"

extern void yyerror(std::string);
int yylex();

%}

NUMBER          [[:digit:]]+
PIDENTIFIER     [_a-z]+
SPECIAL_CHARS   [\;\(\)\:\,]

%x COMMENT

%%
"DECLARE"       { return DECLARE;                               }
"BEGIN"         { return T_BEGIN;                               }
"END"           { return END;                                   }

"ASSIGN"        { return ASSIGN;                                }

"IF"            { return IF;                                    }
"THEN"          { return THEN;                                  }
"ELSE"          { return ELSE;                                  }
"ENDIF"         { return ENDIF;                                 }

"WHILE"         { return WHILE;                                 }
"DO"            { return DO;                                    }
"ENDWHILE"      { return ENDWHILE;                              }
"ENDDO"         { return ENDDO;                                 }
"FOR"           { return FOR;                                   }
"FROM"          { return FROM;                                  }
"TO"            { return TO;                                    }
"DOWNTO"        { return DOWNTO;                                }
"ENDFOR"        { return ENDFOR;                                }

"READ"          { return READ;                                  }
"WRITE"         { return WRITE;                                 }

"PLUS"          { return PLUS;                                  }
"MINUS"         { return MINUS;                                 }
"TIMES"         { return TIMES;                                 }
"DIV"           { return DIV;                                   }
"MOD"           { return MOD;                                   }

"EQ"            { return EQ;                                    }
"NEQ"           { return NEQ;                                   }
"LE"            { return LE;                                    }
"GE"            { return GE;                                    }
"LEQ"           { return LEQ;                                   }
"GEQ"           { return GEQ;                                   }

{NUMBER}        { yylval.num = std::stoll(yytext); return num;  }
{PIDENTIFIER}   { 
                  yylval.pidentifier = new std::string(yytext);
                  return pidentifier; 
                }
{SPECIAL_CHARS} { return yytext[0];                             }

"["             BEGIN(COMMENT);
<COMMENT>{
  "]"           BEGIN(INITIAL);
  "["           yyerror("Nested comment.");
  .             ;
}
\n|\r\n|\r      {                                               }
[[:space:]]     {                                               }
.               { yyerror("Invalid character.");                }

%%
