#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

CONFIG=config.txt

function test_file_exists {
    if [[ ! -f "$1" ]]; then
        echo "Error, file $1 does not exist."
        exit 1
    fi
}

test_file_exists $CONFIG

F1=`cat $CONFIG | grep FICHIER_IN | cut -d ' ' -f 2`
F2=`cat $CONFIG | grep FICHIER_OUT | cut -d ' ' -f 2`

test_file_exists $F1
test_file_exists $F2

diff $F1 $F2 > /dev/null
if [[ $? -eq 0 ]]; then
    echo -e "${GREEN}OK${NC} --> files $F1 and $F2 are identical :-)"
else
    echo -e "${RED}KO${NC} --> files $F1 and $F2 are different!"
fi

exit 0
