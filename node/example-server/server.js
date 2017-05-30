'use strict';

const exec = require('child_process').exec;

function usage()
{
    console.log('Usage: npm start [socket] [data]');
}

function main(process)
{
    if(process.argv.length != 4)
    {
        usage();
        return;
    }

    // start the backend server
    exec('./node_modules/nepomuk/binding/ipc-provider ' + process.argv[3], (err, stdout, stderr) => {
        if (err) {
            console.error(err);
            return;
        }
        console.log(stdout);
    });

    var app = require('./app.js');

    app(process.argv[2]).listen(7777, ()=> {
         console.log(`Listening on 0.0.0.0:7777`);
    });
}

if (require.main === module) { main(process); }
