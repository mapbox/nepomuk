'use strict';

function usage()
{
    console.log('Usage: npm start [dataset:gtfs]');
}

function main(process)
{
    if(process.argv.length != 3)
    {
        usage();
        return;
    }
    var app = require('./app.js');
    app(process.argv[2]).listen(5000, ()=> {
         console.log(`Listening on 0.0.0.0:5000`);
    });
}

if (require.main === module) { main(process); }
