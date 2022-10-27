#!/bin/bash

target=$1
lines=$(wc -l ${target})
read -a tokens <<< "$lines"
echo "${tokens[0]} lines in ${target}"
