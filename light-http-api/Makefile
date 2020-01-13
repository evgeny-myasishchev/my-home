.PHONY: tmp build-env erase_flash write_flash repl

include .build-env

FIRMWARE ?= esp8266-20191220-v1.12.bin
ESP_PORT ?= /dev/cu.usbserial-1420

FIRMWARE_BIN = tmp/$(FIRMWARE)

build-env:
	@echo Generating .build-env. Please edit it as needed
	@echo export FIRMWARE=$(FIRMWARE) > .build-env
	@echo export ESP_PORT=$(ESP_PORT) >> .build-env
	@echo export WIFI_SSID=TODO >> .build-env
	@echo export WIFI_PASSWORD=TODO >> .build-env

erase_flash:
	@esptool.py --port $(ESP_PORT) erase_flash

tmp:
	mkdir -p tmp

$(FIRMWARE_BIN): tmp
	curl http://micropython.org/resources/firmware/$(FIRMWARE) -o $(FIRMWARE_BIN)

write_flash: $(FIRMWARE_BIN)
	@esptool.py --port $(ESP_PORT) --baud 460800 write_flash --flash_size=detect 0 $(FIRMWARE_BIN)

setup_wifi_sta:
	@rshell -p $(ESP_PORT) "repl ~ \
	import network ~\
	import machine ~\
	ap = network.WLAN(network.AP_IF) ~\
	ap.active(False) ~\
	sta = network.WLAN(network.STA_IF) ~\
	sta.active(True) ~\
	sta.connect(\"$(WIFI_SSID)\", \"$(WIFI_PASSWORD)\") ~\
	machine.reset() ~\
	"
wifi_status:
	@rshell -p $(ESP_PORT) "repl ~ \
	import network ~\
	ap = network.WLAN(network.AP_IF) ~\
	print(\"ap active:\", ap.active()) ~\
	sta = network.WLAN(network.STA_IF) ~\
	print(\"sta active:\", sta.active()) ~\
	"

repl:
	@picocom $(ESP_PORT) -b115200