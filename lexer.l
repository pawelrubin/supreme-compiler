%option noyywrap
%option yylineno
%{
#include <string>
#include <iostream>

#include "parser.tab.hpp"

int yylex();
%}

%x COMMENT

%%
\;            { return SEMICOLON;                                   }
\(            { return LB;                                          }
\)            { return RB;                                          }
":"           { return COLON;                                       }
","           { return COMMA;                                       }

"DECLARE"     { return DECLARE;                                     }
"BEGIN"       { return T_BEGIN;                                     }
"END"         { return END;                                         }

"ASSIGN"      { return ASSIGN;                                      }

"IF"          { return IF;                                          }
"THEN"        { return THEN;                                        }
"ELSE"        { return ELSE;                                        }
"ENDIF"       { return ENDIF;                                       }

"WHILE"       { return WHILE;                                       }
"DO"          { return DO;                                          }
"ENDWHILE"    { return ENDWHILE;                                    }
"ENDDO"       { return ENDDO;                                       }
"FOR"         { return FOR;                                         }
"FROM"        { return FROM;                                        }
"TO"          { return TO;                                          }
"DOWNTO"      { return DOWNTO;                                      }
"ENDFOR"      { return ENDFOR;                                      }

"READ"        { return READ;                                        }
"WRITE"       { return WRITE;                                       }

"PLUS"        { return PLUS;                                        }
"MINUS"       { return MINUS;                                       }
"TIMES"       { return TIMES;                                       }
"DIV"         { return DIV;                                         }
"MOD"         { return MOD;                                         }

"EQ"          { return EQ;                                          }
"NEQ"         { return NEQ;                                         }
"LE"          { return LE;                                          }
"GE"          { return GE;                                          }
"LEQ"         { return LEQ;                                         }
"GEQ"         { return GEQ;                                         }

[[:digit:]]+  { yylval.llval  = std::stoll(yytext); return NUM;     }
[_a-z]+       { yylval.strval = strdup(yytext); return PIDENTIFIER; }

"["           BEGIN(COMMENT);
<COMMENT>{
  "]"         BEGIN(INITIAL);
  .           ;
}
\n|\r\n|\r    {                                                     }
[[:space:]]   {                                                     }
.             { 
                std::cerr << yylineno << std::endl; 
                return ERROR_INVALID_CHARACTER;
              }

%%
