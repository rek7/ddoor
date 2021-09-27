#!/bin/bash
SRC_DIR=ddoor
make -C $SRC_DIR
strip bin/binary
