##
## EPITECH PROJECT, 2024
## B-SYN-200-TLS-2-1-automakefile-fabien.fraixanet
## File description:
## skip-synthesis
##

#!/usr/bin/sh

gawk '{print $3,"\t",$9}' | grep \ $1