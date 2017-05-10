'use strict';

var L = require('leaflet');
var lrm = require('leaflet-routing-machine');

class NepomukControl
{
    constructor(map,leaflet_options)
    {
        this.map = map;

        map.on('click', (evnt) => this._add_waypoint(evnt));

        var plan = new L.Routing.Plan([],
                        {
                            routeWhileDragging: true,
                            createMarker: (i,wp,n) => this._get_marker(i,wp,n)
                        });


        var control_options = L.extend(leaflet_options.default_options,{
            routeWhileDragging: true,
            router: L.routing.directionsTransit('access_token_here', leaflet_options),
            plan: plan,
            serviceURL: leaflet_options.services[0].path
        });

        this.lrm_control = L.Routing.control(control_options).addTo(this.map);
    }

    // add onClick event
    _add_waypoint(evnt)
    {
        var num_waypoints = this.lrm_control.getWaypoints()
                                .filter(function(loc){ return loc.latLng; }).length;

        var waypoint = evnt.latlng;

        // for now don't allow setting more than 2 waypoints
        if( num_waypoints == 2 )
            num_waypoints = 1;

        this.lrm_control.spliceWaypoints(num_waypoints,1,waypoint);
        if( num_waypoints <= 1 )
            this.map.panTo(waypoint);
    }

    _get_marker(index, waypoint, num_waypoints)
    {
        var url = 'images/marker-start-icon-2x.png';
        if(index+1 == num_waypoints)
        {
            url = 'images/marker-end-icon-2x.png';
        }
        else if( index > 0 )
        {
            url = 'images/marker-via-icon-2x.png'
        }
        var icon = L.icon({
            iconUrl: url,
            iconSize: [20, 56],
            iconAnchor: [10, 28]
        });

        var marker = L.marker(waypoint.latLng, {draggable: true, icon: icon});

        marker.on('click', (function(lrm_control, index) {
            return function() {
                lrm_control._plan.spliceWaypoints(index, 1);
            }
        })(this.lrm_control, index));

        return marker;
    }
}

module.exports = NepomukControl;
