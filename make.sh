#!/bin/bash
# Compile inform6 and Paa loftet sidder nissen

pwd=$(pwd)

# Compile the Inform6 compiler
cd Inform6-6.31
cc -O2 -o inform *.c
cd $pwd

# Compile the Nissen source file nissen.inf to the storyfile nissen.z5
./Inform6-6.31/inform nissen.inf nissen.z5

# Open nissen.z5 in any z-code interpreter, for example Gargoyle, Lectrote or
# Frotz. See https://www.ifwiki.org/Z-code_interpreters
