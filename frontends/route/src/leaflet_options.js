'use strict';

var L = require('leaflet');

module.exports = {
    services: [{
        label: 'Nepomuk Transit',
        path: 'http://0.0.0.0:5000/directions/v5/nepomuk/transit/eap/'
    }],
    default_options: {
        center: L.latLng(37.7825,-122.4119),
        zoom: 13,
        language: 'en',
        draggableWaypoints: true
    }
};
