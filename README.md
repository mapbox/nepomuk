# Nepomuk

This project offers a library/bindings for running a transit service on GTFS feeds, implemented in modern C++.

[![Build Status](https://travis-ci.com/mapbox/nepomuk.svg?token=bB1pwxscyosgCFnSzzds&branch=master)](https://travis-ci.com/mapbox/nepomuk)
[![codecov](https://codecov.io/gh/mapbox/nepomuk/branch/master/graph/badge.svg?token=yDJlm8LLSU)](https://codecov.io/gh/mapbox/nepomuk)

This project requires valid GTFS feeds to run. Use any of the many available GTFS validators to check if your GTFS dataset qualifies.

## Building

Dependencies of this project are pulled in via Mason. Building it is as simple as checking out the project and running cmake/make.

The main project can be build via cmake.
For example:

```
mkdir build
mkdir build/release

cd build/release
cmake -DCMAKE_BUILD_TYPE=Release ../..
make
```

### Tests

Tests are using ctest. Run `ctest` (or `ctest --verbose`) from within your build directory.

```
cd build/release
ctest
```

## Nodebindings

Nodebindings can be build via node-cmake. To build the bindings, simply specify `-DNODEBINDINGS=On` when configuring the project.

```
mkdir build
mkdir build/release

cd build/release
cmake -DCMAKE_BUILD_TYPE=Release -DNODEBINDINGS=On ../..
make
```

To do so, you need to have node installed in version 4 or later.

```
nvm install 4
nvm use 4
```

### Tests
To test the node bindings, you can run `npm test` from within `node/node-transit/`.
Make sure to run `npm install` beforehand.

```
cd node/node-transit/
npm install
npm test
```

## Frontend/Example Server

The frontend and the example server are provided as a pure example. We highly discourage the usage of both in any production example.

## GTFS sources / validation

There are many GTFS feeds available on the internet. While we will always do our best to verify input to prevent security issues,
we strongly advie anyone to make use of validation tools (E.g. https://developers.google.com/transit/gtfs/guides/tools), or any
of the many available open source checkers.
The validation mechanisms implemented here are no replacement for additional checks of the surrounding API. The example server and it's checks are in no way a recommendation on how to ensure a safe API.
