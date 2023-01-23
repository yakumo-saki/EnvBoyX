#!/bin/bash -eu

WORKDIR=$(dirname $0)

cd $WORKDIR/embed

python -m http.server 8080