const express = require('express');
var transit = require('..');
var cors = require('cors');
var morgan = require('morgan');
const async = require('async');

var corsOptions = {maxAge : 3600e3, origin : '*'};

module.exports =
    function() {
  var app = express();
  // var engine = new transit.Engine("../data/berlin-gtfs");
  var engine = new transit.Engine("../data/sf");
  engine.plug("tile");
  engine.plug("eap");
  // allow same origin and so on
  app.use(cors(corsOptions));
  app.use(morgan(
      '[:date] [info] [http] ":method :url HTTP/:http-version" :status :res[content-length] ":referrer" ":user-agent" ":req[x-amz-cf-id]" :response-time ms'));
  app.use((req, res, next) => { next(); });

  app.get('/directions/v5/:account/:profile/tiles/:z/:x/:y.mvt',
          tile_handler(engine));
  app.get('/directions/v5/:account/:profile/eap/:date/:departure/:waypoints/',
          eap_handler(engine));

  return app;
}

function tile_handler(engine) {
  return (req, res) => {
    var coordinates = [
      parseInt(req.params.z), parseInt(req.params.x), parseInt(req.params.y)
    ];
    var plugin = String("tile");
    engine.request(plugin, coordinates, (err, result) => {
      if (err)
        return res.sendStatus(400);
      res.status(200).type('application/x-protobuf').send(result);
    });
  };
}

function eap_handler(engine) {
  return (req, res) => {
    var date = String(req.params.date);
    var departure = String(req.params.departure);
    var waypoints = (req.params.waypoints || '').split(';');
    if (waypoints.length < 2)
      return next(new ErrorHTTP('Not enough waypoints', 400));

    var matchLonLat = /(-?\d+(?:.\d+)?),(-?\d+(?:.\d+)?)/;
	var coordinates = [];
    var error;

    waypoints.forEach(function(waypoint) {
        var lonLat = waypoint.match(matchLonLat);
        if (lonLat &&
            lonLat[1] >= -180 && lonLat[1] <= 180 &&
            lonLat[2] >= -90 && lonLat[2] <= 90) {
            coordinates.push([lonLat[1], lonLat[2]].map(parseFloat));
        } else {
            error = new ErrorHTTP('Waypoint is invalid', 400);
        }
    });

    if( error )
        return error;

    var plugin = String("eap");
    var parameters = new Object();
    parameters.date = date;
    parameters.departure = departure;
    parameters.coordinates = coordinates;
    engine.request(plugin, parameters, (err, result) => {
      if (err)
        return res.sendStatus(400);
      res.status(200).type('application/json').send(result);
    });
  };
}
