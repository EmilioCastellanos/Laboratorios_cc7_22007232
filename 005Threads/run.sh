#!/bin/bash

echo "Compiling..."

make

if [ $? -eq 0 ]; then
    echo "Running..."
    if [ $# -eq 0 ]; then
        ./log_analyzer
    else
        ./log_analyzer "$1"
    fi
else
    echo "Compilation failed"
fi
