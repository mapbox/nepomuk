# Nepomuk API

Disclaimer: Note that this API is not stable yet and subject to change until we do our first official release.

## Route Descriptions

Many parameters can make up a route. The nepomuk API provides you with a large set of possible values.
We provide the following information about journeys, as far as it is available:

| identifier        | type          | description |
| --------------------- | ------------- | ----------- |
| accessibility         | collection    | Describes the acccessibility for different criteria (wheelchair possible? bikes allowed? elevator?)|
| agency                | collection    | information about the provider |
| arrival/departure     | utc-timestamp | the date and time of a departure/arrival |
| bicycle               | string        | When/If bicycles are allowed {yes | no | timerange} |
| connection            | collection    | available line on the given leg |
| cost                  | number        | Cost of the trip in local currency |
| distance              | number        | Distance travelled in meters |
| duration              | number        | Time travelling along the segment (e.g. walking segments) |
| elevator              | boolean       | Is an elevator present at the station? |
| geometry              | polyline      | the full geometry of a segment / trip |
| headsign              | string        | The posted name of line on the front of the vehicle |
| leg                   | collection    | The description of a single leg as a sequence of stops. A leg is the segment between two waypoints |
| location              | coordinate    | The position of a stop in WGS84 coordinate (lon/lat) |
| mode                  | string        | mode of transportation |
| name (on connecions)  | string        | The name of a line (differs from headsign) |
| short_name            | string        | A short version (e.g. number/code) of a connection name |
| name (on stops)       | string        | The posted name of a stop/station (part of stop)|
| name_color_background | hex           | color code of a line on the map / headsign |
| name_color_text       | hex           | color code of a line on the map / headsign |
| route                 | collection    | The full description of a route: overview, legs, ... |
| segment               | collection    | The undisrupted travel between two stops in one vehicle/by foot |
| stop                  | collection    | All parameters describing a stop |
| summary               | string        | A summary of the trip |
| url                   | url           | a link to a website |
| waypoint              | coordinate    | snapped starting locations on the network (start walking segments / station) |
| wheelchair            | boolean       | Is the vehicle/station equipped to accomodate wheelchais? [true,false] |


### Collections

| collection    | member        | availability  | info |
| ------------- | ------------- | ------------- | ---- |
| response      | code			| guaranteed    | response code 			|
| response      | route         | if code == OK | a list of possible routes |
| response      | waypoints     | guaranteed   	| array of waypoint for each requested location (start,via\*,end) (snappe coordinates) |

| collection    | member        | availability | info |
| ------------- | ------------- | ------------ | ---- |
| route         | accessibility | optional     |      |
| route         | agency        | optional     | if same for the full route |
| route         | arrival       | guaranteed   | duplicated from last leg |
| route         | cost          | optional     | might depend on cost model supplied |
| route         | departure     | guaranteed   | duplicated from first leg |
| route         | duration      | guaranteed   | sum of all legs |
| route         | legs          | guaranteed   | each leg corresponds to a trip between two waypoints |
| route         | summary       | guaranteed   | a summary/description of the route (e.g. most prominent lines) |

| collection    | member        | availability      | info |
| ------------- | ------------- | ----------------- | ---- |
| leg           | accessibility | optional		    |      |
| leg           | agency        | optional          |      |
| leg           | arrival       | guaranteed        | duplicated from last segment |
| leg           | departure     | guaranteed        | duplicated from last segment |
| leg           | duration      | guaranteed        | sum of all segments |
| leg           | segments      | guaranteed        | the segments of the route |

| collection    | member        | availability 	      | info |
| ------------- | ------------- | ------------------- | ---- |
| segment       | arrival       | guaranteed   		  |      |
| segment       | departure     | guaranteed   		  | corresponds to expected connection |
| segment       | duration      | guaranteed   		  |      |
| segment       | geometry      | guaranteed          |      |
| segment       | mode          | guaranteed          | this mimics how a turn instruction (transfer) describes the mode of the upcoming segment |


if segment.mode equals transit:

| collection | member        | availability  | info |
| ---------- | ------------- | ------------- | ---- |
| segment    | accessibility | optional      |      |
| segment    | agency        | optional      |      |
| segment    | connections   | guaranteed    | all connections that service the line, first one is the expected one |
| segment    | cost          | guaranteed    | only on transit segments, if data is available |
| segment    | stops         | optional      | full intermediate stops, corresponds to expected |

if segment.mode equals transfer

| collection | member        | availability  | info |
| ---------- | ------------- | ------------- | ---- |
| segment    | from_station  | guaranteed    |      |
| segment    | to_station    | guaranteed    |      |

if segment.mode equals walk:

| collection | member        | availability  | info |
| ---------- | ------------- | ------------- | ---- |
| segment    | distance      | walking       |      |
> idea: possibly add waiting segments/transfers segments for moments of waiting on a station/transferring trains...

| collection    | member                | availability | info |
| ------------- | --------------------- | ------------ | ---- |
| connection    | arrival               | guaranteed   |      |
| connection    | name_color_background | optional     |      |
| connection    | name_color_text       | optional     |      |
| connection    | departure             | guaranteed   |      |
| connection    | name                  | guaranteed   | name of the connection (line e.g. red line towards Downtown -> red line) |
| connection    | short_name            | optional     | an abbreviated form (e.g. a code) of name |
| connection    | headsign              | guaranteed   | usually describes the destination of the vehicle |

| collection    | member        | availability | info |
| ------------- | ------------- | ------------ | ---- |
| stop          | accessibility | optional     |      |
| stop          | arrival       | guaranteed   |      |
| stop          | departure     | guaranteed   |      | 
| stop          | location      | guaranteed   |      |
| stop          | name          | guaranteed   |      |

| collection    | member        | availability | info |
| ------------- | ------------- | ------------ | ---- |
| accessibility | bicycle       | optional     |      |
| accessibility | elevator      | optional     |      |
| accessibility | wheelchair    | optional     |      |

| collection    | member        | availability | info |
| ------------- | ------------- | ------------ | ---- |
| agency        | name          | guaranteed   | only relevant in regions that offer different providers within a city/or long distance travel (bus agency/train service) |
| agency        | url           | guaranteed   |      |


## Codes

Every response is wrapped next to a `code` variable. Possible responsed for codes are listed here:

### Responses
| Type              | Description                                                                      |
|-------------------|----------------------------------------------------------------------------------|
| `Ok`              | Request could be processed as expected.                                          |
| `InvalidUrl`      | URL string is invalid.                                                           |
| `InvalidService`  | Service name is invalid.                                                         |
| `InvalidVersion`  | Version is not found.                                                            |
| `InvalidOptions`  | Options are invalid.                                                             |
| `InvalidQuery`    | The query string is synctactically malformed.                                    |
| `InvalidValue`    | The successfully parsed query parameters are invalid.                            |
| `InvalidInput`    | Input parameters are invalid                                                     |
| `NoStation`       | One of the supplied input coordinates could not snap to street segment.          |

> these response codes are essentially transferred from OSRM. We need to think about what further codes we could return, if we should emit some of them.

## Basic URL Setup

The basic URL set-up follows the OSRM / Mapbox directions scheme:

```endpoint
GET /{service}/{version}/{profile}/{request}
```

| Parameter   | Description |
| ----------- | --- |
| version     | implemented protocol version. This matches the OSRM version (walking segments) |
| profile     | in our case: transit
| service     | {route/table/isochrone/nearest} |
| request     | specific formatting for the request, based on the used service |

## Route Service

```endpoint
GET /route/v1/transit/{coordinates}[.{format}]?option=value&option=value
```

| Parameter   | Description |
| ----------- | --- |
| coordinates | list of locations. Each location specifies a desired stop along the route. Either `{lon},{lat};{lon},{lat}` or `polyline` |
| format      | optional parameter. Only supports `json` |

> the only mandatory parameter is the coordinates one, similar to an OSRM request. Options specify how the URL is interpreted into the different routing scenarios.
> Route suppors earliest arrivel, latest departure, alternatives. Not mappable so far: profile over a day? Possibly add profile option?

### Options

> These options are what I would like to allow in terms of scenarios.
> From osrm, we only use alternatives, since the meaning transfers quite well to transit. Other prameters are not transferable, right now.
> Some of the parameters are mutually exclusive.


| Option           | Values                       | Description                                                                                           |
|----------------- |------------------------------|-------------------------------------------------------------------------------------------------------|
| alternatives     | `{true\|false}`              | Compute multiple routes, not just the earliest arrival. This will contain a sequence of Pareto-optimal routes (hops/arrival), possibly allow numbers here? |
| arrive           | UTC timestamp                | Arrival of a trip, mutually exclusive to depart |
| depart           | UTC timestamp                | Departure of a trip, mutually exclusive to arrive, NOW if both arrival/depart missing (default to earliest arrival) |
| layovers         | array<int>                   | layovers for all via points, array of size \|waypoints\|-2 |
| walkradius       | `meters <= X`                | Internal, allowed walk radius in the beginning / end of the route -- select dynamically, do not expose in the API |
| walking_speed    | `meters_per_second <= X`     | Internal, prefer/penalise walking, |
| transfer         | `scaling_factor` | Scale timed transfer times, for people running or sneaking |
| avoid            | TYPES...                     | FUTURE OPTION: in some time we should be able to avoid specific (available) modes. This is just for completeness, not going to implement in a first version |


> We are not going to provide walking segments from OSRM, apart from geometry. The walking guidance requires some serious work, still, and can be queried from the firections service.
> We would require some guarantee about snapping, though? Otherwise, it could become difficult to guarantee consistent results.
> Possibly, we could also think about adding transfer distances to allow specifying longer walking segments in between. This is hindered by how walking connections are determined right now. We could offer a upper limit and precompute all walking transfers up to that limit, though and allow lower limits for normal transfers.

#### Example

```curl
curl 'https://api-directions/route/v1/transit/13.388860,52.517037;13.428555,52.523219?overview=false'
```

#### Example Response


## Nearest Service

Find the nearest `number` of stations next to a coordinate. The returned list of stations will not include multiple stations of the same line.

> this `number` version is made to look like osrm. It is possible to request all stations within a radius. In this case, we should probably limit the radius/the number of responses.
> The plugin is not on the short list. The options can be discussed at the time. If we go for radius, it could probably serve to change the name of the API?

```endpoint
GET http://{server}/nearest/v1/{profile}/{coordinates}.json?number={number}
```

|Option      |Values                              |Description                                          |
|------------|------------------------------------|-----------------------------------------------------|
| results    |`10 >= integer >= 1` (default `1`)  | Number of nearest segments that should be returned. |
| radius     | x in [0,LIMIT]                     | Radius in meters for which to return coordinates    |

#### Example

```curl
curl 'https://api-directions/nearest/v1/transit/13.388860,52.517037?results=4'
```

#### Example Response


## Station Service

> Can we use our general geo-coder here? There are many variations we could support that allow finding the best matching station for an error-prone string.
> The result might return a list of probable stations and their names for autocompletion purposes.

Finding a station of a name / geocoding a response based on stations in the network only.

## Timetable Service

For any station, list the number of available services at a given interval

```endpoint
GET http://{server}/timetable/v1/{profile}/{coordinates}.json?options
```

> identifying stations by coordinates should work consistently, since a station (multiple stops) is uniquely identifyable.
> All connections leaving an exact location should be a good result. But we could also think about supplying an ID at some point / hint system.

#### Example

```curl
curl 'https://api-directions/timetable/v1/transit/13.388860,52.517037?results=4'
```

#### Example Response


## Scenarios

### Commute to work

A user wants to ride to/from work. The input consists of the location of the appartment and the workplace.
In this situation, multiple choices are probably the best result. The time to leave for work might not always be timed exactly.
Presenting a number of selections to arrive by a specific point / when starting at a point in time looks promising.
To avoid a stressful commute, we only want to find routes that allow transfers of a minimum time of 3 minutes.

The solution should present a set of 2-3 alternative selections, arriving before / leaving after the specified time.

```Example Request
GET /route/v1/transit/20170401/-122.4885,37.7828;-122.40114,37.78552.json?alternatives=true&arrive=1493112725&transfer=SLOW
```

```Example Result
{
	"code": "Ok",
	"routes": [{
			"arrival": 1491035250,
			"cost": 2.80,
			"departure": 1491033600,
			"duration": 1650,
			"legs": [{
				"departure": 1491033600,
				"arrival": 1491035250,
				"duration": 1650,
				"segments": [{
						"duration": 300,
						"distance": 400,
						"mode": "walking",
						"geometry": "a123",
						"departure": 1491033600,
						"arrival": 1491033900
					},
					{
						"arrival": 1491034950,
						"cost": 2.80,
						"connections": [{
								"departure": 1491033900,
								"arrival": 1491034950,
								"name_color_background": "#FF0000",
								"name_color_text": "#FFFFFF",
								"linename": "red line",
								"headsign": "downtown"
							},
							{
								"departure": 149104500,
								"arrival": 1491035550,
								"name_color_background": "#FF0000",
								"name_color_text": "#FFFFFF",
								"linename": "red line",
								"headsign": "downtown"
							}
						],
						"departure": 1491033900,
						"duration": 1050,
						"stops": [{
								"location": [-122.4885, 37.7830],
								"name": "Main Street"
							},
							{
								"location": [-122.4885, 37.7830],
								"name": "Main Street and 2nd"
							},
							{
								"location": [-122.4885, 37.7830],
								"name": "Main Street and third"
							},
							{
								"location": [-122.40119, 37.78552],
								"name": "Ferry Terminal"
							}
						],
						"geometry": "n01<??123",
						"mode": "bus"
					},
					{
						"duration": 300,
						"distance": 400,
						"mode": "walking",
						"geometry": "a123",
						"departure": 1491034950,
						"arrival": 1491035250
					}
				]
			}],
			"summary": "red line from main street to ferry terminal"
		},
		{
			"arrival": 1491035040,
			"departure": 1491033780,
			"duration": 1260,
			"cost": 3.20,
			"geometry": "a123n01<??123aLL",
			"legs": [{
				"departure": 1491033780,
				"arrival": 1491033900,
				"duration": 120,
				"segments": [{
						"duration": 120,
						"distance": 180,
						"mode": "walking",
						"geometry": "a123",
						"departure": 1491033780,
						"arrival": 1491033900
					},
					{
						"arrival": 1491034320,
						"departure": 1491033900,
						"duration": 420,
						"connections": [{
								"departure": 1491033900,
								"arrival": 1491034320,
								"duration": 420,
								"name_color_background": "#0000FF",
								"name_color_text": "#FFFFFF",
								"linename": "blue line",
								"headsign": "Main Station"
							},
							{
								"departure": 1491033930,
								"arrival": 1491034350,
								"duration": 420,
								"name_color_background": "#00FF00",
								"name_color_text": "#FFFFFF",
								"linename": "green line",
								"headsign": "downtown"
							}
						],
						"stops": [{
								"location": [-122.4885, 37.7830],
								"name": "Side Street"
							},
							{
								"location": [-122.4885, 37.6830],
								"name": "Main Station"
							}
						],
						"geometry": "n01<??123",
						"mode": "bus"
					},
					{
						"name": "white line",
						"departure": 1491034380,
						"arrival": 1491034740,
						"duration": 360,
						"conncetions": [{
							"departure": 1491034380,
							"arrival": 1491034740,
							"duration": 360,
							"name_color_background": "#FFFFFF",
							"name_color_text": "#000000"
						}],
						"stops": [{
								"location": [-122.4885, 37.6830],
								"name": "Main Station"
							},
							{
								"location": [-122.40119, 37.78552],
								"name": "Ferry Terminal"
							}
						],
						"geometry": "n01<??123",
						"mode": "bart"
					},
					{
						"departure": 1491034740,
						"arrival": 1491035040,
						"duration": 300,
						"distance": 400,
						"mode": "walking",
						"geometry": "aLL"
					}
				]
			}],
			"summary": "blue and white line via main station"
		}
	],
	"waypoints": [{
			"location": [-122.4885, 37.7828]
		},
		{
			"location": [-122.40114, 37.78552]
		}
	]
}
```

### Scenario: Buying some Wine on the Trip to your Aunt

On the way to your aunts birthday, you want to stop by a fancy Whinery to buy a bottle of Wine for dinner.
The trip starts at your house, stops near the Whinery and continues onwards to your aunt. You decide that 10 minutes should be good enough to get a bottle of wine and be back on your way.

```Example Request
GET /route/v1/transit/13.388860,52.517037;13.397634,52.529407;13.428555,52.523219&alternatives=false&depart=1493112725&layovers=600
```

```Example Result
TODO: Trip Walk -> Transit -> Walk -- 10 minutes delay -- Walk -> Transit -> Walk
```

### Scenario Delivery

Using public transport to deliver goods. We have node-or-tools. Requires distance table (approx) during normal operation hours.
This scenario is only realistic in major cities with a regular schedule. We have node-or-tools for this now. For smaller set-ups, we should be able to fake a matrix with profile queries.
The plugin should simply mimic a matrix api.

```Example Request
GET /table/v1/transit/13.388860,52.517037;13.397634,52.529407;13.428555,52.523219?depart=1493112725
```

```Example Result
{
    "code": "Ok",
    "durations": [0,1600,2200,1600,0,600,2200,600,0]
}
```

### Isochrones

A business wants to know the reach of a location via public transit to target a marketing campaign. The owner wants to know where potential customers could come frome, if he assumes a half an hour time for transit.
The owner uses the histogram service to find a polygon of all reachable locations within X minutes of public tranist/walking distance.
The result of the request is a multi polygon that describes the set of rechable entries.

Additional parameter: radius -- define the timeframe for the isochrone

```Example Request
GET /isochrones/v1/transit/-122.40114,37.78552.json?depart=1493112725&radius=1800
```

```Example Result
  {
  	"code": "Ok",
  	"hulls": [{
  			"duration": 60,
  			"outline": [
  				[-122.40915298461914,
  					37.78950581068895
  				],
  				[-122.4111270904541,
  					37.78496111569183
  				],
  				[-122.40520477294922,
  					37.784757615348575
  				],
  				[-122.40134239196777,
  					37.78075532805877
  				],
  				[-122.39473342895508,
  					37.78197638783258
  				],
  				[-122.39421844482422,
  					37.786860425207784
  				],
  				[-122.39679336547852,
  					37.79031975640008
  				],
  				[-122.39928245544434,
  					37.792218928191865
  				],
  				[-122.40520477294922,
  					37.79031975640008
  				],
  				[-122.40795135498047,
  					37.793643275000754
  				],
  				[-122.40915298461914,
  					37.78950581068895
  				]
  			]
  		},
  		{
  			"duration": 100,
  			"outline": [
  				[-122.40863800048828,
  					37.78496111569183
  				],
  				[-122.4111270904541,
  					37.78496111569183
  				],
  				[-122.4103546142578,
  					37.78652126637423
  				],
  				[-122.41310119628906,
  					37.789098834471204
  				],
  				[-122.4203109741211,
  					37.7846897817763
  				],
  				[-122.41052627563477,
  					37.77973776283843
  				],
  				[-122.40863800048828,
  					37.78496111569183
  				]
  			]
  		},
  		{
  			"duration": 100,
  			"outline": [
  				[-122.40220069885254,
  					37.79357544958489
  				],
  				[-122.3997116088867,
  					37.796559708979885
  				],
  				[-122.40391731262207,
  					37.79717011082383
  				],
  				[-122.40614891052246,
  					37.79486412183905
  				],
  				[-122.40220069885254,
  					37.79357544958489
  				]
  			]
  		}
  	]
  }
```


