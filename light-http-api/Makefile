.PHONY: build-env erase_flash write_flash repl sync_code setup_wifi_sta wifi_status shell test
.PHONY: fetch_config push_config push_addresses

include .build-env
include .vendor-versions

PLATFORM ?= esp8266
FIRMWARE ?= $(PLATFORM)-20191220-v1.12.bin
ESP_PORT ?= /dev/cu.usbserial-1420

FIRMWARE_BIN := tmp/$(FIRMWARE)

build-env:
	@echo Generating .build-env. Please edit it as needed
	@echo export PLATFORM=$(PLATFORM) > .build-env
	@echo export FIRMWARE=$(FIRMWARE) > .build-env
	@echo export ESP_PORT=$(ESP_PORT) >> .build-env
	@echo export WIFI_AP_SSID=TODO >> .build-env
	@echo export WIFI_AP_PASSWORD=TODO >> .build-env
	@echo export WIFI_STA_SSID=TODO >> .build-env
	@echo export WIFI_STA_PASSWORD=TODO >> .build-env

erase_flash:
	@esptool.py --port $(ESP_PORT) erase_flash

tmp:
	mkdir -p tmp

$(FIRMWARE_BIN): tmp
	curl http://micropython.org/resources/firmware/$(FIRMWARE) -o $(FIRMWARE_BIN)

write_flash: $(FIRMWARE_BIN)
	@esptool.py --port $(ESP_PORT) --baud 460800 write_flash --flash_size=detect 0 $(FIRMWARE_BIN)

vendor/uuid.py: .vendor-versions
	mkdir -p vendor/uuid
	curl https://raw.githubusercontent.com/pfalcon/pycopy-lib/$(PYCOPY_LIB_UUID_VERSION)/uuid/uuid.py -o vendor/uuid.py

vendor: vendor/uuid.py

fetch_config:
	@rshell -p $(ESP_PORT) cp /pyboard/config/platform-$(PLATFORM).json config/platform-$(PLATFORM).json

push_config:
	@rshell -p $(ESP_PORT) mkdir -p /pyboard/config
	@rshell -p $(ESP_PORT) cp config/platform-$(PLATFORM).json /pyboard/config/platform-$(PLATFORM).json

push_addresses:
	@rshell -p $(ESP_PORT) cp config/addresses.json /pyboard/config/addresses.json

shell:
	@rshell -p $(ESP_PORT) shell

sync_code: 
	@rshell -p $(ESP_PORT) rsync -m ./src /pyboard/src
	@rshell -p $(ESP_PORT) rsync -m ./vendor /pyboard/vendor

sync_boot: 
	@rshell -p $(ESP_PORT) cp boot.py /pyboard/boot.py

sync_main: 
	@rshell -p $(ESP_PORT) cp main.py /pyboard/main.py

setup_wifi_sta:
	@pyboard --device $(ESP_PORT) -c 'import wifi;wifi.setupSTA("$(WIFI_STA_SSID)", "$(WIFI_STA_PASSWORD)")'

setup_wifi_ap:
	@pyboard --device $(ESP_PORT) -c 'import wifi;wifi.setupAP("$(WIFI_AP_SSID)", "$(WIFI_AP_PASSWORD)")'

wifi_status:
	@pyboard --device $(ESP_PORT) -c 'import wifi;wifi.status()'

repl:
	@rshell -p $(ESP_PORT) repl

test:
	@bin/run-tests.py