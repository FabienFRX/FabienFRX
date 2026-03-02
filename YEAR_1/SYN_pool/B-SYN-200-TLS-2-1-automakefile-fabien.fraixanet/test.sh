#!/bin/bash
values="test test1 test2 test3"
directory="src"
srcs=""
sources=$(echo $values | tr " " "\n" )
echo $sources
for source in $sources; do
    echo "test: ${source}"
    srcs+=" ${directory}/${source}"
done
echo $srcs