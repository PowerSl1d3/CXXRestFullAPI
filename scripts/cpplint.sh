#!/bin/bash

cpplint --linelength=120 ../main.cpp 1> log.txt 2>/dev/null
number_of_errors=$(($(grep -w "Total" log.txt | grep -oP "\b[0-9]+") + 0))
rm log.txt
if (( number_of_errors > 10 )); then
  echo "Too many errors(${number_of_errors})! Please, fix your code"
  exit 1
fi
echo "Good quality of code! Good job!"
exit 0