const express = require('express');
var transit = require('..');
var cors = require('cors');
var morgan = require('morgan');
const async = require('async');

var corsOptions = {
    maxAge: 3600e3,
    origin: '*'
};

module.exports = function()
{
    var app = express();
    var engine = new transit.Engine("../data/berlin-gtfs");
    engine.plug("tile");

    // allow same origin and so on
    app.use(cors(corsOptions));
    app.use(morgan('[:date] [info] [http] ":method :url HTTP/:http-version" :status :res[content-length] ":referrer" ":user-agent" ":req[x-amz-cf-id]" :response-time ms'));
    app.use((req, res, next) => {
      	next();
    });

    app.get('/directions/v5/:account/:profile/tiles/:z/:x/:y.mvt', tile_handler(engine));

    return app;
}


function tile_handler(engine) {
  return (req, res) => {
	var coordinates = [parseInt(req.params.z),parseInt(req.params.x),parseInt(req.params.y)];
	var plugin = String("tile");
	engine.request(plugin,coordinates,(err, result) => {
      	if (err)
        	return res.sendStatus(400);
        res.status(200).type('application/x-protobuf').send(result);
    });
  };
}

