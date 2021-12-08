# Shell #

This program implements the execution of kernel commands. This is a prototype of shell, that execute system calls.

## 1.Compilation ##
``gcc 1.c -o 1 -Wall -fsanitize=address``

## 2.Stages ##
  There are 9 stages:
  
    - Tokenization
    
    - Standard start
    - I / O redirection
      `` f.e. ls -l > 1.txt``
      
    - Pipeline for two command
       `` f.e ls | wc``
    - Pipeline for arbitrary number of command
        `` f.e. ls | wc > 2.txt | pwd ``
    - Background mode
        ``f.e. ls &
            date``
    - Change directory
       `` f.e. cd - or cd or cd home``
    - Pipeline && and or||
       `` f.e. pwd && ls
            pwd || ls ``
    - Ctrl + C

## 3.Meta ##
My name is Olga Snissarenko
