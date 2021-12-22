# Shell #

This program implements the execution of kernel commands. This is a prototype of shell, that execute system calls.

## 1.Compilation ##
use Makefile

## 2.Stages ##
  There are 9 stages:
  
  1. Tokenization
  2. Standard start
  3. I / O redirection 
      f.e.  `` ls -l > 1.txt``
  4. Pipeline for two command  
      f.e `` ls | wc``
  5. Pipeline for arbitrary number of command  
      f.e. `` ls | wc > 2.txt | pwd ``
  6.  Background mode       
      f.e. `` ls &
            date``
  7. Change directory 
      f.e. `` cd - or cd or cd home``
  8. Pipeline && and or||
      f.e.  ``pwd && ls
            pwd || ls ``
  9. Ctrl + C


## 3.Meta ##
My name is Olga Snissarenko
