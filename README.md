## UID: 605745935

## Pipe Up

Our goal is to implement a program with the same functionality as the pipe operator, `|`, that uses multiple processes to redirect input and output from linux commands. 

## Building

Explain briefly how to build your program

We iterate through the list of arguments provided by user input. We handle special cases with only 1 or 2 arguments separately. For each argument, we'll create a pipe buffer and fork the process. 
Taking the input from a previous argument(process), we'll redirect the file descriptors of our processes to read/write from the pipe buffer instead of standard input/output. This, of course, changes in the first and last argument cases as they will read/write from standard input/output respectively. We also ensure to `wait()` for child processes in the parent code in order to avoid orphan processes. 

## Running

Show an example run of your program, using at least two additional arguments, and what to expect

First, we run the `make` command to create our pipe executable

Then, we can run something like `./pipe ls wc cat` which should replicate the results of running the commands `ls | wc | cat`

This should take the bytes of the current directory, count the number of words(bytes) and print them out. ???

## Cleaning up

Explain briefly how to clean up all binary files

To clean up all binary files, we'll use the command `make clean`
