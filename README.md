# EXPGen

# Experience from raw data

This project simulates a simple game: a line of number.

- Action 0 does nothing
- Action 1 increments first value, goes back to 0 if at 3
- Action 2 shifts all the values to the left

With this demonstration, we are able to understand the properties of values and their dependencies.
We just input random moves until we understand what's going on.

This method is not optimal, we should isolate specific actions at specific moments depending on the goals.
This is meant to locate patterns among the values by determining each value properties.

Another set is created to reward the system, which corresponds to the sum of the values.
Best reward is then: "333"

A mapping (flowchart) can be generated using the conditions that were established to do so.


Libraries to install:

sudo apt install g++


Command to launch:

g++ pattern.cpp search.cpp expgen.cpp utils.cpp line.cpp core.cpp deduction.cpp -o expgen
