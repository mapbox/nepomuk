#!/bin/bash

set -o errexit

if [ ! -d "./node_modules/node-cmake" ]; then
    npm install npm install node-cmake@2.2.1 --unsafe-perm
fi

if [ ! -d "./node_modules/nan" ]; then
    npm install nan@2.5.1 --unsafe-perm
fi

mkdir -p ./node/nepomuk/binding/

./node_modules/node-cmake/lib/ncmake.js update;
