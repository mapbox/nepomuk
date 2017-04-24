(function() {
	'use strict';

	var L = require('leaflet');
    require('leaflet-routing-machine');
	var corslite = require('corslite');
	var polyline = require('polyline');

	L.Routing = L.Routing || {};

	L.Routing.DirectionsTransit = L.Class.extend({
		options: {
			serviceUrl: 'http://0.0.0.0:5000/directions/v5/pumba/transit/eap/',
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
                        totalDistance: responseRoute.distance,
                        totalTime: responseRoute.duration
                    }
                },
                legNames = [],
                index = 0,
                legCount = responseRoute.legs.length,
                hasSteps = responseRoute.legs[0].steps.length > 0,
                i,
                j,
                leg,
                step,
                geometry,
                type,
                modifier;

            for (i = 0; i < legCount; i++) {
                leg = responseRoute.legs[i];
                legNames.push(leg.summary && leg.summary.charAt(0).toUpperCase() + leg.summary.substring(1));
                for (j = 0; j < leg.steps.length; j++) {
                    step = leg.steps[j];
                    geometry = this._decodePolyline(step.geometry);
                    result.coordinates.push.apply(result.coordinates, geometry);
                    type = step.maneuver;

                    if (type) {
                        result.instructions.push({
                            type: type,
                            modifier: "straight",
                            distance: step.distance,
                            time: step.duration,
                            road: step.name,
                            index: index,
                            mode: step.mode,
                        });
                    }
					index += geometry.length;
                }
            }

            result.name = legNames.join(', ');
            if (!hasSteps) {
                result.coordinates = this._decodePolyline(responseRoute.geometry);
            }

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
				/* Instructions are always needed, 
				   since we do not have waypoint indices otherwise */
				true,
				//!(options && options.geometryOnly),
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
		},

		_mapWaypointIndices: function(waypoints, instructions, coordinates) {
			var wps = [],
				wpIndices = [],
			    i,
			    idx;

			wpIndices.push(0);
			wps.push(new L.Routing.Waypoint(coordinates[0], waypoints[0].name));

			for (i = 0; instructions && i < instructions.length; i++) {
				if (instructions[i].sign === 5) { // VIA_REACHED
					idx = instructions[i].interval[0];
					wpIndices.push(idx);
					wps.push({
						latLng: coordinates[idx],
						name: waypoints[wps.length + 1].name
					});
				}
			}

			wpIndices.push(coordinates.length - 1);
			wps.push({
				latLng: coordinates[coordinates.length - 1],
				name: waypoints[waypoints.length - 1].name
			});

			return {
				waypointIndices: wpIndices,
				waypoints: wps
			};
		}
	});

	L.Routing.directionsTransit = function(apiKey, options) {
		return new L.Routing.DirectionsTransit(apiKey, options);
	};

	module.exports = L.Routing.DirectionsTransit;
})();
