#!/bin/bash

WORKDIR=$(dirname $0)

cd $WORKDIR/embed

watch -n 15 cp -v "./static/*" .