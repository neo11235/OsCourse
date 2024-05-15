#!/bin/bash
# script takes 4 arguments that are given to the master worker program
echo "compiling master-worker.c"
gcc -o master-worker master-worker.c -lpthread
echo "running master-worker with parameter $1  $2 $3  $4" 
./master-worker $1 $2 $3 $4 > output 
echo "running awk file"
awk -f check.awk MAX=$1 output
