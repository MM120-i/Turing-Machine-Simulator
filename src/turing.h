#include <stdbool.h>

#ifndef __turing_h__
#define __turing_h__

#define MAX_TRANSITIONS 5
#define MAX_STATES 25
#define MAX_STATE_NAME_LEN 15

struct State;
struct Transition;

typedef struct Transition Transition;
typedef struct State State;

typedef enum
{
    LEFT,
    RIGHT
} Direction;

struct Transition
{
    struct State *start;
    struct State *next;
    Direction move;
    char input;
    char wrtie;
};

struct State
{
    char name[MAX_STATE_NAME_LEN];
    struct Transition *transitions[MAX_TRANSITIONS];
    int transitionCount;
    bool accept;
    bool reject;
};

struct Turing
{
    State *states[MAX_STATES];
    State *current;
    State *reject;
    State *accept;
    int stateCount;
    int head;
};

typedef struct Turing Turing;

Transition *TransitionCreate(State *, State *, Direction, char, char);
void TransitionDestory(Transition *);
void TransitionPrint(Transition *);
State *StateCreate(const char *, bool, bool);
bool StateAddTransition(State *, Transition *);
void StatePrint(State *);
void StateDestory(State *);
Turing *TuringCreate();
void TuringDestory(Turing *);
bool TuringAddState(Turing *, State *);
void TuringPrint(Turing *, char *, int);
State *TuringStep(Turing *, char *, int);
void TuringRun(Turing *, char *, int);

#endif