var tap = require('tap');
var nepomuk = require('..');
var fs = require('fs');

const exec = require('child_process').exec;

'use strict';

tap.test('Require Socket', function(assert) {
    assert.throws(() =>new nepomuk.Engine(), 'Create requires a single parameter (the socket ipc-provider is listening on)', 'Should throw an error');
    assert.throws(() =>new nepomuk.Engine({test: 123123}), 'Parameter to create needs to be a single string.', 'Should throw an error');
    assert.throws(() => nepomuk.Engine('ipc:///tmp/non-construct').request('tile',[0,0],()=>{}), 'Parameter must be an array [z, x, y]');
    assert.end();
})

tap.test('Invalid Parameters', function(assert) {
    var engine = new nepomuk.Engine('ipc:///tmp/nepomuk-invalid-parameters-no-plugin');
	var route_parameters = {
        coordinates: [[0.005, 0.0049],[0.0353, 0.005]],
        departure: 0,
        walking_radius: 1500.0
    };
	var invalid_route_parameters = {
        coordinates: [[0.0353, 0.005]],
        departure: 0
    };


    assert.throws(() => engine.request(route_parameters, (err, result) => {}), 'First parameter needs to be the plugin name.', 'Should throw an error');
    assert.throws(() => engine.request(route_parameters, 'route', (err, result) => {}), ' First parameter needs to be the plugin name', 'Should throw an error');
    assert.throws(() => engine.request('route', 1, (err,result) => {}),  'Parameters need to be an object', 'Should throw an error');
    assert.throws(() => engine.request('route', route_parameters, route_parameters),  'Last parameter needs to be the callback', 'Should throw an error');
    assert.throws(() => engine.request('route', invalid_route_parameters, (err,result) => {}),  'Route Queries need to specify exactly two locations', 'Should throw an error');
    assert.throws(() => engine.request('tile', [12,1], (err,result) => {}),  'Parameter must be an array [z, x, y]');
    assert.throws(() => engine.request('garbage', route_parameters, (err,result) => {}),  'The plugin \'garbage\' isn\'t implemented.', 'Should throw an error');

    // tile ids
    assert.throws(() => engine.request('tile', [1, 12,12], (err,result) => {}),  '[error] failed request due to: Invalid tile coordinates', 'Should throw an error');
    assert.throws(() => engine.request('tile', ['twelve', 12,12], (err,result) => {}),  'Tile z coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', [12,'twelve',12], (err,result) => {}),  'Tile x coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', [12,12,'twelve'], (err,result) => {}),  'Tile y coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', [undefined,12,12], (err,result) => {}),  'Tile z coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', [12,undefined,12], (err,result) => {}),  'Tile x coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', [12,12,undefined], (err,result) => {}),  'Tile y coordinate must be unsigned interger');
    assert.throws(() => engine.request('tile', {z: 12, x:12, y:12}, (err,result) => {}),  'Parameter must be an array [z, x, y]');

    // invalid values in route request
    assert.throws(() => engine.request('route', {coordinates: [[0.0353, 0.005],[0.0353, 0.005]]}, (err,result) => {}),  'Missing one of departure/arrival');
    assert.throws(() => engine.request('route', {coordinates: [[0.0353, 0.005],[0.0353, 0.005]], departure: 0, arrival: 0}, (err,result) => {}),  'Cannot specify both departure and arrival time.');
    assert.throws(() => engine.request('route', {coordinates: [[0.0353, 0.005],[0.0353, 0.005]], departure: 'NOW'}, (err,result) => {}),  'Expecting Number.');
    assert.throws(() => engine.request('route', {coordinates: [[0.0353, 0.005],[0.0353, 0.005]], departure: 0, walking_radius: 'three meters'}, (err,result) => {}),  'Expecting Number.');
    // needs to be invalid in two ways, since we don't open a backend service. Walking_Radius undefined is checked prior to having both arrival/departure
    assert.throws(() => engine.request('route', {coordinates: [[0.0353, 0.005],[0.0353, 0.005]], departure: 0, arrival: 0, walking_radius: undefined}, (err,result) => {}),  'Expecting Number.');

    // parse coordinates
    assert.throws(() => engine.request('route', {departure: 0}, (err,result) => {}),  'Failed to request coordinates. Did you specify correct coordinates?');
    assert.throws(() => engine.request('route', {coordinates: [], departure: 0}, (err,result) => {}),  'Failed to request coordinates. Did you specify correct coordinates?');
    assert.throws(() => engine.request('route', {coordinates: [[],[]], departure: 0}, (err,result) => {}),  'Found empty coordinate set');
    assert.throws(() => engine.request('route', {coordinates: {from: [0.0,0.0], to: [1.0,1.0]}, departure: 0}, (err,result) => {}),  'Coordinates has to be an array of lon/lat pairs.');
    assert.throws(() => engine.request('route', {coordinates: undefined, departure: 0}, (err,result) => {}),  'Must provide a coordinates property');
    assert.throws(() => engine.request('route', {coordinates: [[0],[1]], departure: 0}, (err,result) => {}),  'Coordinates must be an array of (lon/lat) pairs');
    assert.throws(() => engine.request('route', {coordinates: [['eins',2],[0,1]], departure: 0}, (err,result) => {}),  'Each member of a coordinate pair must be a number');
    assert.throws(() => engine.request('route', {coordinates: [[1,'zwei'],[0,1]], departure: 0}, (err,result) => {}),  'Each member of a coordinate pair must be a number');
    assert.throws(() => engine.request('route', {coordinates: [[undefined,2],[0,1]], departure: 0}, (err,result) => {}),  'Each member of a coordinate pair must be a number');
    assert.throws(() => engine.request('route', {coordinates: [[1,undefined],[0,1]], departure: 0}, (err,result) => {}),  'Each member of a coordinate pair must be a number');
    assert.throws(() => engine.request('route', {coordinates: [{x: 0, y:0},[0,1]], departure: 0}, (err,result) => {}),  'Coordinates must be an array of (lon/lat) pairs');
    assert.throws(() => engine.request('route', {coordinates: [[1/0,0],[0,1]], departure: 0}, (err,result) => {}),  'Lon/Lat coordinates must be valid numbers');
    assert.throws(() => engine.request('route', {coordinates: [[181,80],[80,80]], departure: 0}, (err,result) => {}),  'Lon/Lat coordinates must be within WGS84 bounds (-180 < lng < 180, -85.05 < lat < 85.05)');
    assert.throws(() => engine.request('route', {coordinates: [[179,88],[80,80]], departure: 0}, (err,result) => {}),  'Lon/Lat coordinates must be within WGS84 bounds (-180 < lng < 180, -85.05 < lat < 85.05)');

    // shutdown requests work
    assert.throws(() => engine.send_shutdown('route'),  'Last parameter needs to be the callback', 'Should throw an error');
    assert.throws(() => engine.send_shutdown(),  'Parameter missmatch: requiring only callback', 'Should throw an error');

    assert.end();
})
