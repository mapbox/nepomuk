# Nepomuk example-server

This is a simple example server for Nepomuk transit. It is, to no extent, ready for use in a production service.
It is provided as a pure example in how to set up a running Nepomuk backend server.

## Usage

First install the server by running `npm install`

Download any GTFS dataset. We recommend getting the San Francisco Municipal Transit feed (e.g. https://transitfeeds.com/p/sfmta/60), lets say into `data/sf`.

Run the backend server using `npm start data/sf`.
