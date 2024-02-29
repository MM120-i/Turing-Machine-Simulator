#ifndef __turingparser_h__
#define __turingparser_h__

Transition *TransitionParse(FILE *, State **, int);
State *StateParse(FILE *);
Turing *TuringParse(FILE *);

#endif