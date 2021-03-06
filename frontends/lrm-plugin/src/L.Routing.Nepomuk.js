(function() {
	'use strict';

	var L = require('leaflet');
    require('leaflet-routing-machine');
	var corslite = require('corslite');
	var polyline = require('polyline');

	L.Routing = L.Routing || {};
    L.routing = L.routing || {}

	L.Routing.Nepomuk = L.Class.extend({
		options: {
			serviceUrl: 'http://0.0.0.0:5000/directions/v5/nepomuk/transit/eap/',
			timeout: 30 * 1000,
			urlParameters: {walking_radius: 1500,
                            walking_speed: 2,
                            departure: 1491048000,
                            transfer_scale: 1.0}
		},

		initialize: function(apiKey, options) {
			this._apiKey = apiKey;
			L.Util.setOptions(this, options);
		},

		route: function(waypoints, callback, context, options) {
			var timedOut = false,
				wps = [],
				url,
				timer,
				wp,
				i;

			options = options || {};
			url = this.buildRouteUrl(waypoints, options);

			timer = setTimeout(function() {
								timedOut = true;
								callback.call(context || callback, {
									status: -1,
									message: 'Direction Transit request timed out.'
								});
							}, this.options.timeout);

			// Create a copy of the waypoints, since they
			// might otherwise be asynchronously modified while
			// the request is being processed.
			for (i = 0; i < waypoints.length; i++) {
				wp = waypoints[i];
				wps.push({
					latLng: wp.latLng,
					name: wp.name,
					options: wp.options
				});
			}

			corslite(url, L.bind(function(err, resp) {
				var data;

				clearTimeout(timer);
				if (!timedOut) {
					if (!err) {
						data = JSON.parse(resp.responseText);
						this._routeDone(data, wps, options, callback, context);
					} else {
						var finalResponse;
						var responseText = err && err.responseText;
						try {
							finalResponse = JSON.parse(responseText);
						} catch (e) {
							finalResponse = responseText;
						}

						callback.call(context || callback, {
							status: -1,
							message: 'HTTP request failed: ' + err,
							response: finalResponse
						});
					}
				}
			}, this));

			return this;
		},

        _toWaypoints: function(inputWaypoints, vias) {
            var wps = [],
                i,
                viaLoc;
            for (i = 0; i < vias.length; i++) {
                viaLoc = vias[i].location;
                wps.push(L.Routing.waypoint(L.latLng(viaLoc[1], viaLoc[0]),
                                            inputWaypoints[i].name,
                                            inputWaypoints[i].options));
            }

            return wps;
        },

		_routeDone: function(response, inputWaypoints, options, callback, context) {
			var alts = [],
			    mappedWaypoints,
			    i,
				route;

			context = context || callback;
            if (response.code !== 'Ok') {
                callback.call(context, {
                    status: response.code
                });
                return;
            }

			mappedWaypoints = this._toWaypoints(inputWaypoints, response.waypoints);

            for (i = 0; i < response.routes.length; i++) {
                route = this._convertRoute(response.routes[i]);
                route.inputWaypoints = inputWaypoints;
                route.waypoints = mappedWaypoints;
                route.properties = {isSimplified: !options || !options.geometryOnly || options.simplifyGeometry};
                alts.push(route);
            }

			callback.call(context, null, alts);
		},

		_decodePolyline: function(geometry) {
			var coords = polyline.decode(geometry, 5),
				latlngs = new Array(coords.length),
				i;
			for (i = 0; i < coords.length; i++) {
				latlngs[i] = new L.LatLng(coords[i][0], coords[i][1]);
			}

			return latlngs;
		},

		_convertRoute: function(responseRoute) {
            var result = {
                    name: '',
                    coordinates: [],
                    instructions: [],
                    summary: {
                        totalDistance: 0,
                        totalTime: responseRoute.duration
                    }
                },
                legNames = [],
                index = 0,
                legCount = responseRoute.legs.length,
                i,
                j,
                k,
                leg,
                segment,
                geometry,
                modifiier;

            result.instructions.push({
                type: 'walk',
                stop: responseRoute.legs[0].segments[0].stops[0].name,
                index: 0
            });

            for (i = 0; i < legCount; i++) {
                leg = responseRoute.legs[i];
                for (j = 0; j < leg.segments.length; j++) {
                    segment = leg.segments[j];
                    geometry = this._decodePolyline(segment.geometry);
                    result.coordinates.push.apply(result.coordinates, geometry);

                    // transit segment
                    if( segment.connections )
                    {
                        // transfer at the end of the line
                        if( j > 0 && leg.segments[j-1].connections )
                        {
                            result.instructions.push({
                                type: 'transfer',
                                origin: leg.segments[j-1].stops[leg.segments[j-1].stops.length-1].name,
                                destination: segment.stops[0].name,
                                index: index
                            });
                        }
                        result.instructions.push({
                            type: 'transit',
                            time: segment.duration,
                            index: index,
                            headsign: segment.connections[0].headsign,
                            name: segment.connections[0].name
                        });
                    } else if (segment.distance) {
                        result.instructions.push({
                            type: 'walk',
                            distance: segment.distance,
                            time: segment.duration,
                            index: index
                        });
                    } else {
                        result.instructions.push({
                            type: 'transfer',
                            time: segment.duration,
                            origin: segment.origin.name,
                            destination: segment.destination.name,
                            index: index
                        });
                    }
					index += geometry.length;
                }
            }

            var last_leg = responseRoute.legs[responseRoute.legs.length - 1];
            var last_segment = last_leg.segments[last_leg.segments.length - 1];
            var last_stop = last_segment.stops[last_segment.stops.length-1];
            result.instructions.push({
                type: 'walk',
                stop: last_stop.name,
                index: index
            });

            return result;
        },

		_toWaypoints: function(inputWaypoints, vias) {
			var wps = [],
			    i;
			for (i = 0; i < vias.length; i++) {
				wps.push({
					latLng: L.latLng(vias[i]),
					name: inputWaypoints[i].name,
					options: inputWaypoints[i].options
				});
			}

			return wps;
		},

		buildRouteUrl: function(waypoints, options) {
			var computeInstructions =
				true,
				locs = [],
				i,
				baseUrl;
			
			for (i = 0; i < waypoints.length; i++) {
				locs.push(waypoints[i].latLng.lng + ',' + waypoints[i].latLng.lat);
			}

			baseUrl = this.options.serviceUrl
				+ locs.join(';');

			return baseUrl + L.Util.getParamString(L.extend({
					instructions: computeInstructions,
					type: 'json',
					key: this._apiKey
				}, this.options.urlParameters), baseUrl);
		}
	});

	L.Routing.nepomuk = function(apiKey, options) {
		return new L.Routing.Nepomuk(apiKey, options);
	};

    L.routing.nepomuk = L.Routing.nepomuk;

	module.exports = L.Routing.Nepomuk;
})();
