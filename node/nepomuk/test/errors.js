var tap = require('tap');
var nepomuk = require('..');
var fs = require('fs');

const exec = require('child_process').exec;

'use strict';

tap.test('Require Socket', function(assert) {
    assert.throws(() =>new nepomuk.Engine(), 'Create requires a single parameter (the socket ipc-provider is listening on)', 'Should throw an error');
    assert.throws(() =>new nepomuk.Engine({test: 123123}), 'Parameter to create needs to be a single string.', 'Should throw an error');
    assert.end();
})

tap.test('Inbalid Parameters', function(assert) {
    var engine = new nepomuk.Engine('ipc:///tmp/nepomuk-no-plugin');
	var route_parameters = {
        coordinates: [[0.005, 0.0049],[0.0353, 0.005]],
        departure: 0
    };
	var invalid_route_parameters = {
        coordinates: [[0.0353, 0.005]],
        departure: 0
    };


    assert.throws(() => engine.request(route_parameters, (err, result) => {}), 'First parameter needs to be the plugin name.', 'Should throw an error');
    assert.throws(() => engine.request(route_parameters, "route", (err, result) => {}), ' First parameter needs to be the plugin name', 'Should throw an error');
    assert.throws(() => engine.request("route", 1, (err,result) => {}),  'Parameters need to be an object', 'Should throw an error');
    assert.throws(() => engine.request("route", route_parameters, route_parameters),  'Last parameter needs to be the callback', 'Should throw an error');
    assert.throws(() => engine.request("route", invalid_route_parameters, (err,result) => {}),  'Route Queries need to specify exactly two locations', 'Should throw an error');
    assert.throws(() => engine.request("tile", [12,1], (err,result) => {}),  'Last parameter needs to be the callback', 'Should throw an error');
    assert.throws(() => engine.request("tile", [1, 12,12], (err,result) => {}),  '[error] failed request due to: Invalid tile coordinates', 'Should throw an error');
    assert.throws(() => engine.request("garbage", route_parameters, (err,result) => {}),  'The plugin \"garbage\" isn\'t implemented.', 'Should throw an error');


    assert.throws(() => engine.send_shutdown("route"),  'Last parameter needs to be the callback', 'Should throw an error');
    assert.throws(() => engine.send_shutdown(),  'Parameter missmatch: requiring only callback', 'Should throw an error');

    assert.end();
})
