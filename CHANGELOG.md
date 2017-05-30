# NEPOMUK 0.0.0

  - support for parsing GTFS feeds from csv (see https://developers.google.com/transit/gtfs/)
  - debug front-end using mapbox vector tiles specification 2.0
    - includes detection of strongly connected components (SCC) via Tarjans algorithm
    - visualises available transfers
  - leaflet routing machine plugin for transit routing
    - comes with a simple example front-end
  - support for native module node bindings via NAN
    - with a basic eaxmple server
  - initial routing functionality
  - API proposal documentation
  - support for look-ups from coordinates to stations
  - implement IPC infastructure for serving requests in PBF form. Connect any language to Nepomuk that offers IPC and protobuf
