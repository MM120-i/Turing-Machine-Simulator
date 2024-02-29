#include "turing.h"
#include "error.h"
#include "turingparser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_LINE_LEN 80

Transition *TransitionParse(FILE *fh, State **states, int stateLength)
{
    State *st1 = NULL;
    State *st2 = NULL;

    int parsed = 0;

    char s1[MAX_STATE_NAME_LEN], s2[MAX_STATE_NAME_LEN];
    char format[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];
    char input, move, write;
    Direction dir;

    fgets(buffer, MAX_LINE_LEN, fh);

    if (feof(fh))
    {
        return NULL;
    }

    sprintf(format, "%%%ds %%c -> %%%ds %%c %%c", MAX_STATE_NAME_LEN - 1, MAX_STATE_NAME_LEN - 1);
    parsed = sscanf(buffer, format, s1, &input, s2, &write, &move);

    if (parsed < 5)
    {
        log_err("Wrong translation line in file. ");
        goto out;
    }

    for (size_t i = 0; i < stateLength; i++)
    {
        State *state = states[i];

        if (strcmp(state->name, s1) == 0)
        {
            st1 = state;
        }
        else if (strcmp(state->name, s2) == 0)
        {
            st2 = state;
        }
    }

    if (!st1 || !st2)
    {
        log_err("parse error: could not find the states of one of the transition specified.");
        goto out;
    }

    switch (move)
    {
    case 'L':
        dir = LEFT;
        break;

    case 'R':
        dir = RIGHT;
        break;

    default:
        log_err("parse error: could not parse direction of the transitions.");
        goto out;
    }

    Transition *transition = TransitionCreate(st1, st2, dir, input, write);

    if (!StateAddTransition(st1, transition))
    {
        log_err("Failed to add state transition.");
        goto out;
    }

    return transition;

out:
    return NULL;
}

State *StateParse(FILE *fh)
{
    int parsed, accept, reject;
    char name[MAX_STATE_NAME_LEN];
    char mode = '\0';
    char format[MAX_LINE_LEN];
    char buffer[MAX_LINE_LEN];

    fgets(buffer, MAX_LINE_LEN, fh);

    sprintf(format, "%%%ds %%c", (MAX_STATE_NAME_LEN - 1)), parsed = sscanf(buffer, format, name, &mode);

    if (parsed < 1)
    {
        log_err("Could not parse state line.");
        goto out;
    }

    switch (mode)
    {
    case 'A':
        accept = true;
        reject = false;
        break;

    case 'R':
        accept = false;
        reject = true;
        break;

    default:
        accept = false;
        reject = false;
    }

    State *state = StateCreate(name, accept, reject);
    assert(state);
    return state;

out:
    return NULL;
}

Turing *TuringParse(FILE *fh)
{
    int numStates, parsed;
    char buffer[MAX_LINE_LEN];

    Turing *machine = TuringCreate();

    while (true)
    {
        fgets(buffer, MAX_LINE_LEN, fh);

        if (feof(fh))
        {
            log_err("Inavlid input file");
            goto out;
        }
        else if (buffer[0] != '#')
        {
            break;
        }
    }

    parsed = sscanf(buffer, "%d", &numStates);

    if (parsed < 1)
    {
        log_err("Could not parse the state number.");
        goto out;
    }

    for (size_t i = 0; i < numStates; i++)
    {
        State *state = StateParse(fh);

        if (!state)
        {
            goto out;
        }

        if (!TuringAddState(machine, state))
        {
            log_err("Failed to add state.");
            goto out;
        }
    }

    while (1)
    {
        Transition *transition = TransitionParse(fh, machine->states, machine->stateCount);

        if (!transition)
        {
            break;
        }
    }

    machine->current = machine->states[2];

    if (!machine->current)
    {
        log_err("Turing machine should have atleast one state.");
        goto out;
    }

    return machine;

out:

    TuringDestory(machine);
    return NULL;
}