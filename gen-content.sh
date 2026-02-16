#!/bin/sh

cd webgen && ./main -d .. && cd ..

cd site && zip -r ../site.zip .
