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
			return String("Take " + instr.name + " towards " + instr.headsign);
		}
		else if( instr.type == "transfer")
		{
			return String("Transfer at " + instr.stopname);
		}
		else
		{
			if( i == 0 )
				return String("Walk towards station");
			else
				return String("Walk towards destination");
		}
	}
});

L.Routing.nepomukFormatter = function() {
	return new L.Routing.NepomukFormatter();
};

L.routing.nepomukFormatter = L.Routing.nepomukFormatter;

module.exports = L.Routing.NepomukFormatter;

})();
