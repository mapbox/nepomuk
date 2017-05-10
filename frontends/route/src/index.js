'use strict';

var L = require('leaflet');
require('leaflet-routing-machine'); // inject .routing into L
require('lrm-directions-transit'); // inject directions transit plugin into L.routing
require('./nepomuk_formatter.js'); // inject own formatter into L.routing.formatter

var NepomukControl = require('./nepomuk_control.js');
var leaflet_configuration = require('./leaflet_options.js');

// load our mapbox layers
var mapbox_layers = require('./mapbox-layers')

// set up the basic map
var map = L.map('map').setView(leaflet_configuration.default_options.center,
                               leaflet_configuration.default_options.zoom);

// select mapbox streets
mapbox_layers.baselayer.one.addTo(map);

var control_options = L.extend(leaflet_configuration.default_options,{
    routeWhileDragging: true,
	router: L.routing.directionsTransit('access_token_here', {}),
    formatter: L.routing.nepomukFormatter(),
});

var nepomuk_control = new NepomukControl(map, leaflet_configuration);




