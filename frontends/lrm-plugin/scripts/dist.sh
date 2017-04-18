#!/bin/bash

VERSION=`echo "console.log(require('./package.json').version)" | node`

echo Building dist files for $VERSION...
mkdir -p dist
browserify -t browserify-shim src/L.Routing.DirectionsTransit.js >dist/lrm-directions-transit.js
uglifyjs dist/lrm-directions-transit.js >dist/lrm-directions-transit.min.js
echo Done.
