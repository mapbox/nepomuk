const express = require('express');
var nepomuk = require('nepomuk');
var cors = require('cors');
var morgan = require('morgan');
const async = require('async');

var corsOptions = {maxAge : 3600e3, origin : '*'};

module.exports =
  function(dataset) {
  var app = express();
  var engine = new nepomuk.Engine(dataset);

  // register plugins
  engine.plug("tile");
  engine.plug("eap");

  // allow same origin and so on
  app.use(cors(corsOptions));
  app.use(morgan(
      '[:date] [info] [http] ":method :url HTTP/:http-version" :status :res[content-length] ":referrer" ":user-agent" ":req[x-amz-cf-id]" :response-time ms'));
  app.use((req, res, next) => { next(); });

  // register different urls to their services
  app.get('/directions/v5/:account/:profile/tiles/:z/:x/:y.mvt',
          tile_handler(engine));
  app.get('/directions/v5/:account/:profile/eap/:waypoints/',
          eap_handler(engine));

  return app;
}

function tile_handler(engine) {
  // handle a tile request
  return (req, res) => {
    var coordinates = [
      parseInt(req.params.z), parseInt(req.params.x), parseInt(req.params.y)
    ];
    var plugin = String("tile");
    engine.request(plugin, coordinates, (err, result) => {
      if (err)
          return res.status(400).send('Bad Request: ' + err);
      res.status(200).type('application/x-protobuf').send(result);
    });
  };
}

function eap_handler(engine) {
  // handle a route request
  return (req, res) => {
    var departure = String(req.params.departure);
    var waypoints = (req.params.waypoints || '').split(';');
    if (waypoints.length < 2)
      return res.status(400).send('Not enough waypoints');

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
            error = res.status(400).send('Waypoint is invalid');
        }
    });

    if( error )
        return error;

    var plugin = String("eap");
    var parameters =
    {
        coordinates: coordinates,
        departure: (req.query.departure !== undefined) ? parseInt(req.query.departure) : undefined,
        arrival: (req.query.arrival !== undefined) ? parseInt(req.query.arrival) : undefined,
        walking_radius: parseFloat(req.query.walking_radius),
        walking_speed: parseFloat(req.query.walking_speed),
        transfer_scale: parseFloat(req.query.transfer_scale)
    }

    engine.request(plugin, parameters, (err, result) => {
      if (err)
      {
        return res.status(400).send('Bad Request: ' + err);
      }
      res.status(200).type('application/json').send(result);
    });
  };
}
