CSCI 4401: Programming Assignment 1

Author: Ted Mader

A simple shell using the POSIX system call API.

To run the project, use the following commands in the terminal:

1. make
2. ./mash.exe

Prompt
--
Once started, the user will be given a prompt that shows the current working directory (e.g.: /home/csci4401>), and the shell enters an infinite loop where it takes user commands and executes them.

Built-in Commands
--
Mash implements the cd and pwd commands that allow a user to change the current directory and to print the current directory, respectively. The exit command allows orderly shell exit.

Simple Program Execution
--
Unless the user types in a built-in command, any other input will be interpreted as an effort to run an executable. In response, Mash will fork() a process, execute the command and show the output (e.g.: ls -l).

Stream Redirection
--
Mash allows redirection of standard output/ input streams to/from a file. E.g:
* ls -l > output
* sort alice.txt > sorted.txt o sort < alice.txt > sorted.txt

Pipes
--
Mash allows IPC via pipes. E.g.: ls -l | grep sh | wc -l