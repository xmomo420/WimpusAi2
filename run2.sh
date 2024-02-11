#!/bin/sh

# Variables
JEU="Jeu"
EXE="$(basename "$(pwd)")"

# Exécution
./"$JEU" "$@" ./"$EXE"
