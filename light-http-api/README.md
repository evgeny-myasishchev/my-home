# ESP Home API

ESP8266 firmware to control [light-controller](../light-controller/README.md)

# API

The API will be communicating with Arduino via UART. A subset of AT commands will be used to send or request some data.

## GET /ping

`curl http://<ip>/ping`

Responds with 
```json
{"api": "OK", "controller": "OK" }
````

AT command:

```AT+PING```

AT response:

```OK```

## GET /pins/{pinNumber}

Get state of a corresponding pin.

`curl http://<ip>/pins/20`

The response is:
```json
{"state": 1 }
````

AT command: 

```AT+PIN?```

AT response:

```
+PIN:<state>
OK
```

## POST /pins/{pin} body: state={state}

Set state of a corresponding pin. The state can be 0 or 1.

`curl -XPOST http://<ip>/pins/20 -d state=1`

AT command: 

```AT+PIN=<pin>,<state>```

AT response:

```
OK
```

## GET /pins

Get state of all pins. Example response is below:

```json
{"bytes": 2, "state": 255 }
````

AT command: 

```AT+PINS?```

AT response:

```
+PINS=<bytes>,<state>
OK
```

## POST /pins body: state={state}

Set state of all pins. The {state} is a number that corresponds to bytes of all pins.

`curl -XPOST http://<ip>/pins -d state=255`

AT command: 

```AT+PINS=<state>```

AT response:

```
OK
```