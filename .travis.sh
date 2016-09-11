#!/bin/bash
REVISION=$(git rev-parse --short HEAD)
make clean
make
