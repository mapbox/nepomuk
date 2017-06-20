(function() {

'use strict';

var L = require('leaflet');
require('leaflet-routing-machine');

L.Routing = L.Routing || {};
L.routing = L.routing || {}
L.Routing.Formatter = L.Routing.Formatter || {};
L.routing.formatter = L.routing.formatter || {};

L.Routing.NepomukFormatter = L.Routing.Formatter.extend({
	_getInstructionTemplate: function(instr, i) {
		if( instr.type == "transit" )
		{
			return String("Take the " + instr.name + " line towards " + instr.headsign);
		}
		else if( instr.type == "transfer")
		{
            if( instr.origin != instr.destination)
    			return String("At " + instr.origin + " transfer to " + instr.destination);
            else
                return String("Transfer at " + instr.destination);
		}
		else
		{
			if( i == 0 )
				return String("Walk towards " + instr.stop);
			else
				return String("Exit at " + instr.stop + " and Walk towards your destination");
		}
	}
});

L.Routing.nepomukFormatter = function() {
	return new L.Routing.NepomukFormatter();
};

L.routing.nepomukFormatter = L.Routing.nepomukFormatter;

module.exports = L.Routing.NepomukFormatter;

})();
