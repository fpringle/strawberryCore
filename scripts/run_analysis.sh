#!/usr/bin/bash

function help {
    echo "Usage: $0 [-b|--board-string] STRING [OPTIONS]"
    echo "OPTIONS:"
    echo "  -d N"
    echo "  --depth N: specify a search depth N"
    exit 1
}


BOARD="default"
DEPTH="-1"

while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        -b|--board-string)
            BOARD="$2"
            shift
            shift
            ;;
        -d|--depth)
            DEPTH="$2"
            shift
            shift
            ;;
        *)
            help
            ;;
    esac
done


make analyse

./analyse.exe "$BOARD" "$DEPTH" | ./py_analyse.py

