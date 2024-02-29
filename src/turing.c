#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "turing.h"
#include "turingparser.h"

#define NDEBUG
#include "error.h"

static int verbose = 1;

Transition *TransitionCreate(State *start, State *next, Direction move, char input, char write)
{
    assert(start);
    assert(next);

    Transition *transition = malloc(sizeof(Transition));

    if (!transition)
    {
        log_err("Memory error");
        return NULL;
    }

    transition->start = start;
    transition->next = next;
    transition->move = move;
    transition->input = input;
    transition->wrtie = write;

    assert(transition);

    return transition;
}

void TransitionDestory(Transition *transition)
{
    free(transition);
}

void TransitionPrint(Transition *transition)
{
    if (!transition)
    {
        printf("Transition {NULL}");
    }
    else
    {
        printf("Transition{\n\tstate: %s\n\tinput: %c\n\tnet: %s\n\twrite: %c\n\tmove: %d\n}\n", transition->start->name, transition->input, transition->next->name, transition->wrtie, transition->move);
    }
}

State *StateCreate(const char *name, bool accept, bool reject)
{
    assert(name);

    State *state = malloc(sizeof(State));

    if (!state)
    {
        log_err("Memory error");
        return NULL;
    }

    strncpy(state->name, name, MAX_STATE_NAME_LEN - 1);
    state->name[MAX_STATE_NAME_LEN - 1] = '\0';

    state->accept = accept;
    state->reject = reject;
    state->transitionCount = 0;

    assert(state);

    return state;
}

bool StateAddTransition(State *state, Transition *transition)
{
    assert(state);
    asset(transition);

    if (state->transitionCount == MAX_TRANSITIONS)
    {
        log_err("State %s already has the maximum amount of trnasitions.", state->name);
        return false;
    }

    state->transitions[state->transitionCount] = transition;
    state->transitionCount = state->transitionCount + 1;

    return true;
}

void StatePrint(State *state)
{
    if (!state)
    {
        printf("State {NULL}");
    }
    else
    {
        printf("State{\n\tname: %s\n\taccept: %d\n\treject: %d\n}\n", state->name, state->accept, state->reject);
    }
}

void StateDestory(State *state)
{
    assert(state);

    for (size_t i = 0; i < state->transitionCount; i++)
    {
        Transition *transition = state->transitions[i];

        if (!transition)
        {
            log_err("Problem destorying state transition.");
            continue;
        }

        TransitionDestory(transition);
    }

    free(state);
}

Turing *TuringCreate()
{
    Turing *machine = malloc(sizeof(Turing));

    if (!machine)
    {
        log_err("Memory error.");
        return NULL;
    }

    machine->stateCount = 0;
    machine->current = NULL;
    machine->head = 0;

    machine->accept = StateCreate("accpet", true, false);
    machine->reject = StateCreate("reject", false, true);
    TuringAddState(machine, machine->accept);
    TuringAddState(machine, machine->reject);

    assert(machine);

    return machine;
}

void TuringDestory(Turing *machine)
{
    assert(machine);

    for (size_t i = 0; i < machine->stateCount; i++)
    {
        State *state = machine->states[i];

        if (!state)
        {
            log_err("Error while destorying machine state.");
            continue;
        }

        StateDestory(state);
    }

    free(machine);
}

bool TuringAddState(Turing *machine, State *state)
{
    assert(machine);
    assert(state);

    if (machine->stateCount == MAX_STATES)
    {
        log_err("The turing machine already has the maximum amount of states.");
        return false;
    }

    machine->states[machine->stateCount++] = state;
    return true;
}

void TuringPrint(Turing *machine, char *tape, int tapeLength)
{
    assert(machine);
    assert(tape);

    for (size_t i = 0; i < machine->head; i++)
    {
        printf("%c", tape[i]);
    }

    State *currentState = machine->current;

    if (!currentState)
    {
        log_err("Failed to get the machine's current state(s).");
        return;
    }

    printf(" %s ", currentState->name);

    for (size_t i = machine->head; i < tapeLength; i++)
    {
        printf("%c", tape[i]);
    }

    printf("\n");
}

State *TuringStep(Turing *machine, char *tape, int tapeLength)
{
    assert(machine);
    assert(tape);

    char input = tape[machine->head];
    State *state = machine->current;

    for (size_t i = 0; i < state->transitionCount; i++)
    {
        Transition *transition = state->transitions[i];

        if (!transition)
        {
            log_err("Error while trying to get state transition.");
            continue;
        }

        if (transition->input == input)
        {
            State *next = transition->next;

            if (!next)
            {
                log_err("Transition to NULL state.");
                goto error;
            }

            if (transition->wrtie != '\\')
            {
                tape[machine->head] = transition->wrtie;
            }

            if (transition->move == LEFT)
            {
                if (machine->head > 0)
                {
                    machine->head--;
                }
            }
            else
            {
                if (machine->head + 1 >= tapeLength)
                {
                    log_err("Machine walked of tape on right side.");
                    goto error;
                }
                machine->head++;
            }

            machine->current = next;
            assert(next);
            return next;
        }
    }

    return machine->reject;

error:
    return NULL;
}

void TuringRun(Turing *machine, char *tape, int tapeLength)
{
    assert(machine);
    assert(tape);

    if (!machine->current)
    {
        log_err("Turing machine has now started state.");
        goto error;
    }

    while (1)
    {
        State *state = TuringStep(machine, tape, tapeLength);

        if (!state)
        {
            goto error;
        }

        if (state->accept)
        {
            printf("\n> Done!\n\n> Input accepted in state: %s\n", state->name);
            break;
        }
        else if (state->reject)
        {
            printf("\n> Done!\n\n> Input rejected in state: %s\n", state->name);
            break;
        }
        else
        {
            if (verbose > 0)
            {
                TuringPrint(machine, tape, tapeLength);
            }
        }
    }

error:
    return;
}

int main(int argc, char *argv[])
{
    int c;
    extern int optind, optopt;

    while ((c = getopt(argc, argv, ":s") != -1))
    {
        switch (c)
        {
        case 's':
            verbose = 0;
            break;
        case '?':
            printf("unknown option: -%c\n", optopt);
            break;

        default:
            break;
        }
    }

    if (argc < 4)
    {
        die("USAGE: $ turing <FILE> <tape length> <tape init>");
    }

    char *fname = argv[optind++];
    int tapeLength = atoi(argv[optind++]);

    char *tape = (char *)malloc((tapeLength + 1) * sizeof(char));

    if (tape == NULL)
    {
        fprinf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    strncpy(tape, argv[optind++], tapeLength);
    tape[tapeLength] = '\0';

    printf(fname);
    FILE *fh = fopen(fname, "r");

    if (!fh)
    {
        die("Could not open file for reading");
    }

    printf("> Parsing file %s ... \n\n", fname);

    Turing *machine = TuringParse(fh);

    if (!machine)
    {
        die("Failed parsing the machine, exiting.");
    }

    printf("> Starting simulation... \n\n");

    if (verbose > 0)
    {
        TuringPrint(machine, tape, tapeLength);
    }

    TuringRun(machine, tape, tapeLength);

    fclose(fh);
    TuringDestory(machine);
    free(tape);

    return 0;
}
