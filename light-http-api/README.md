TODO: Try https://github.com/pfalcon/picoweb

# ESP Home API

ESP8266 firmware to control [light-controller](../light-controller/README.md)

# Dependencies

```
# esptool
pip install esptool

# PlatformIO
pip install -U platformio

# rshell
pip3 install rshell
```

# Configure build system

Generate build config file
```
make build-env
```

Edit generated .build-env file. Following variables available:

* **FIRMWARE** is a name of a firmware file to flash (if flashing). 
* **ESP_PORT** is a port where ESP is connected. Can be found with `pio device list`
* **WIFI_AP_SSID** AP SSID if you want your board to operate in AP mode
* **WIFI_AP_PASSWORD** corresponding AP password
* **WIFI_STA_SSID** STA SSID if you want your board to operate in STA mode (e.g client) 
* **WIFI_STA_PASSWORD** corresponding STA password

Currently available firmware can be found [here](http://micropython.org/download#esp8266)

# Prepare board

## Flash micropython firmware

```
# Erase the flash
make erase_flash

# Write a new flash. Note: you may have to reset the board
make write_flash
```

## Config

Copy `config/template.json` to `config/platform-<platform>.json` where `<platform>` is value of `sys.platform`.

Some platforms:
* OSX - `darwin`
* ESP8266 - `esp8266`

```
# To fetch existing config
make fetch_config

# To write new config
make push_config
```

## Sync code

```
make sync_boot
```
Do it once and if you ever change boot.py

```
make sync_main
```
Do it once and if you ever change main.py

```
make sync_code
```
You may have to do it periodically to upload a new code

## Setup networking

Make sure to have .build-env setup

```
# Connect to a wifi network (STA mode)
make setup_wifi_sta

# Or setup AP network
make setup_wifi_ap

# Check wifi status
make wifi_status
```

## Running locally

Optional dependencies:
* [direnv](https://github.com/direnv/direnv)
* [reflex](https://github.com/cespare/reflex)

Install [micropython](https://github.com/micropython/micropython#the-unix-version)

Install deps tests
```
micropython -m upip install micropython-unittest
micropython -m upip install micropython-random
```

Unittest framework for micropython is quite silent. In order to see more details you need to edit `<lib>/unittest.py` and uncomment `raise` on line 204. Usually `<lib>` is a ${HOME}/.micropython/lib/.

Run unit tests
```
# All tests
make test

# One file only
bin/run-tests.py -f tests/test_server.py
```

Optionally use reflex to watch changes and restart tests:
```
# Directly
reflex $(cat .reflex) -- bin/run-tests.py

# With a helper script (assume direnv installed)
tests-watch

# Single file
tests-watch -f tests/test_server.py
```

# Debugging

## UDP logger transport hints

Listen for UDP port with `nc`:
```
nc -kluw 0 0.0.0.0 50001 | npx pino-pretty
```

# API

The API will be communicating with Arduino via UART. A subset of AT commands will be used to send or request some data.

## GET /v1/light/ping

`curl http://<ip>/ping`

Responds with 
```json
{"service": "PONG", "controller": "OK" }
````

AT command:

```AT+PING```

AT response:

```OK```

## GET /v1/light/pins/{pinNumber}

Get state of a corresponding pin.

`curl http://<ip>/pins/20`

The response is:
```json
{"state": 1 }
````

AT command: 

```AT+PIN?=<pinNumber>```

AT response:

```
+PIN:<state>
OK
```

## POST /v1/light/pins/{pin} body: state={state}

Set state of a corresponding pin. The state can be 0 or 1.

`curl -XPOST http://<ip>/pins/20 -d { "state": 1 }`

AT command: 

```AT+PIN=<pin>,<state>```

AT response:

```
OK
```

## GET /v1/light/pins

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