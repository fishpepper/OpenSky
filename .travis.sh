#!/bin/bash
touch .i_know_what_i_am_doing
REVISION=$(git rev-parse --short HEAD)
make clean
make
