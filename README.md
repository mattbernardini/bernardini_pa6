bernardini_pa6
These programs are designed to work together.  Runsim.c creates runsim which is the creator process.
Testsim.c creates the child processes that are forked from runsim.

TO USE:

Requires:
    One argument that is the number of concurrent processes to allow at a time
    A file with commands in it to process, can have no more than 2 commands
        An example would be testsim 5 10

The exit status' are pulled from the system and do not nessecialrliy reflect the child exit status