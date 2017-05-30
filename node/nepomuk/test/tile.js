var tap = require('tap');
var nepomuk = require('..');
var fs = require('fs');

const exec = require('child_process').exec;

'use strict';

tap.test('Test Route No Route', function(assert) {
    const socket_id = 'ipc:///tmp/nepomuk_npm_test_tile';
    exec('./binding/ipc-provider ./test/fixtures/three_lines ' + socket_id, (err, stdout, stderr) => {
        if (err) {
            console.error(err); // eslint-disable-line no-console
            return;
        }
        console.log(stdout); // eslint-disable-line no-console
    });

    var engine = new nepomuk.Engine(socket_id);

	var tile_parameters = [15, 16384, 16383];

    engine.request("tile", tile_parameters, (err, result) => {
        tap.error(err);
        engine.send_shutdown((err, result) =>
        {
            assert.end();
        });
    });
})

