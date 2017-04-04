# Transit 0.0.0

  - added a csv parser for gtfs messages (see https://developers.google.com/transit/gtfs/)
  - specified format for gtfs protocol buffer messages (see https://developers.google.com/protocol-buffers/)
  - provides scc computation for transit networks, including a plain adaptor into a connectivity graph
  - added mapbox vector tile 2.0 implementaiton
  - basic routing functionality implemented
  - basic geometric features for wgs84/web-mercator (coordinate + bounding box). Including wgs84 -> web-mercator
  - added node bindings to expose plugin architecture
  - added basic frontend for debug tiles
  - added look-ups (plain, computational expensive) for coordinate to stop, stop to trip, stop to station
  - implemented some GTFS clean-up routines
    - implemented filtering unused stops to avoid look-up pollution
    - connect close stops that share a name into stations
