#!/bin/bash

./configure CFLAGS='-m32 -O2 -U_FORTIFY_SOURCE -fno-PIC' CXXFLAGS='-m32 -O2 -U_FOTIFY_SOURCE -fno-PIC' LDFLAGS=-m32 --disable-shared duck
make
