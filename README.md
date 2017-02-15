# directions-transit
This repository is going to host everything for a Mapbox public transit api in terms of backend / bindings


## Where this is heading

- ingesting/updating transit timetable data, probably in GTFS format
- calculating routes that
  - calculating transit costs
  - optimize for transit time
  - time-dependent questions
  - considering different transpotation modes (bus or train only)
  - integrating walk legs with `api-directions`
  - consider real-time transit data? (delays, schedule changes?)
  - other stuff that we don't know about yet? Please help me scope this out more.
- designing and implementing the API surface area, and building documentation for it
- getting it deployed and publishing the service
