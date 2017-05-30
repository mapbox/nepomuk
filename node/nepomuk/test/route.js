var tap = require('tap');
var nepomuk = require('..');
var fs = require('fs');

const exec = require('child_process').exec;

'use strict';

tap.test('Test Route No Route', function(assert) {
    const socket_id = 'ipc:///tmp/nepomuk_npm_test_routes_no_route';
    exec('./binding/ipc-provider ./test/fixtures/three_lines ' + socket_id, (err, stdout, stderr) => {
        if (err) {
            console.error(err); // eslint-disable-line no-console
            return;
        }
        console.log(stdout); // eslint-disable-line no-console
    });

    var engine = new nepomuk.Engine(socket_id);

	var route_parameters = {
        coordinates: [[0.005, 0.0049],[0.0353, 0.005]],
        departure: 1492127999
    };

    engine.request("route", route_parameters, (err, result) => {
        assert.equal(err.message,'No Route');
        engine.send_shutdown((err, result) =>
        {
            assert.end();
        });
    });
})

tap.test('Test Route', function(assert) {
    var a1_d = JSON.parse(fs.readFileSync(__dirname + '/../test/fixtures/three_lines/a1_d.json'));
    const socket_id = 'ipc:///tmp/nepomuk_npm_test_routes';
    exec('./binding/ipc-provider ./test/fixtures/three_lines ' + socket_id, (err, stdout, stderr) => {
        if (err) {
            console.error(err); // eslint-disable-line no-console
            return;
        }
        console.log(stdout); // eslint-disable-line no-console
    });

    var engine = new nepomuk.Engine(socket_id);

	var route_parameters = {
        coordinates: [[0.005, 0.0049],[0.0353, 0.005]],
        departure: 0
    };

    engine.request("route", route_parameters, (err, result) => {
        tap.error(err);
        try{
            var json_result = JSON.parse(result);
            assert.deep_equal(json_result,a1_d);
        } catch (exception)
        {
            assert.fail('Failed parsing of: ' + result);
        }
        engine.send_shutdown((err, result) =>
        {
            assert.end();
        });
    });
})
