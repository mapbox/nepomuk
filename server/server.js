'use strict';

function main()
{
    var app = require('./app.js');
    app().listen(5000, ()=> {
         console.log(`Listening on 0.0.0.0:5000`);
    });
}

if (require.main === module) { main(); }
