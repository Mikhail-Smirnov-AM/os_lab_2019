#!/bin/bash

if [[ -n $1 ]]
then 
  count=0
  sum=0
  
  case "$1" in 
  -a) shift
      while [ -n "$1" ]
      do
      sum=$[$sum + $1]
      count=$[$count + 1]
      shift
      done
      ;;
  -f) for line in `cat $2` 
      do 
      sum=$[$sum + line]
      count=$[$count + 1] 
      done
      ;;
  *) echo "Нет опции $1" ;;
  esac
  
  if [[ $count -eq 0 ]]
  then 
    echo "Количество равно нулю."
  else
    echo "Количество = $count."
    echo "Среднее арифметическое = $(jq -n $sum/$count)."
    
  fi
else
    echo "Не выбрана опция"
fi