# Turing Machine Simulator

## Overview

This project provides a simple implementation of a Turing Machine Simulator in C. The simulator reads a Turing Machine description from a file, processes an input tape, and simulates the behavior of the machine.

## Files

- `turing.c`: Main program that reads the Turing Machine description, processes the input tape, and runs the simulation.
- `turingparser.c` and `turingparser.h`: Contains functions for parsing a Turing Machine from a file.
- `error.c` and `error.h`: Provides error handling functions used throughout the program.
- `turing.h`: Header file defining structures and functions related to the Turing Machine.
- `Makefile`: Makefile for building the project.

## Build and Run

To build the Turing Machine Simulator, run the following commands in the terminal:

```bash
make
```

This will generate the executable `turing`. To run the simulator, use the following command:

```bash
./turing <FILE> <tape length> <tape init>
```

- `<FILE>`: Path to the file containing the Turing Machine description.
- `<tape length>`: Length of the input tape.
- `<tape init>`: Initial content of the input tape.

Optional flag `-s` can be used to suppress verbose output during simulation.

## Turing Machine Description File

The Turing Machine description file specifies the states, transitions, and behavior of the Turing Machine. The file format is designed to be simple, with each line representing a transition.

Example:

```
state1 0 state2 1 R
state2 1 state1 0 L
...
```

Each line represents a transition in the format:

```
current_state input next_state write_direction
```

- `current_state`: The current state of the machine.
- `input`: The input symbol read from the tape.
- `next_state`: The next state to transition to.
- `write_direction`: The direction to move the tape head (`R` for right, `L` for left).

  Inspiaration was taken from https://codereview.stackexchange.com/questions/19814/simple-turing-machine-simulator
