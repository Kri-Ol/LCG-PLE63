#!/bin/sh

gfortran -c global.f90
gfortran -c lcg.f90
gfortran -o main main.f90 global.o lcg.o

