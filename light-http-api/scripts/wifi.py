import sys
# import machine

# ap = network.WLAN(network.AP_IF)
# ap.active(False)
# sta = network.WLAN(network.STA_IF)
# sta.active(True)
# sta.connect("$(WIFI_SSID)", "$(WIFI_PASSWORD)")
# machine.reset()

def showHelp():
    print("Wifi management. Commands: setup-ap, setup-sta, status, help")
    print("Args: --ssid <ssid>, --password <password> (for setup commands only)")

def status():
    import network
    sta = network.WLAN(network.STA_IF)
    ap = network.WLAN(network.AP_IF)
    if sta.active(): 
        print("STA active")
        print("ifconfig", sta.ifconfig())
        print("essid", sta.config('essid'))
    if ap.active():
        print("AP active")
        print("ifconfig", ap.ifconfig())
        print("essid", ap.config('essid'))
        print("authmode", ap.config('authmode'))

def setupSTA(ssid, password):
    import network
    import machine
    sta = network.WLAN(network.STA_IF)
    ap = network.WLAN(network.AP_IF)
    if not sta.active(): 
        print("STA active. Deactivating...")
        sta.active(True)
    sta.connect(ssid, password)
    if ap.active():
        print("AP is not active, activating....")
        ap.active(False)
    machine.reset()

def setupAP(ssid, password):
    import network
    import machine
    sta = network.WLAN(network.STA_IF)
    ap = network.WLAN(network.AP_IF)
    if sta.active(): 
        print("STA active. Deactivating...")
        sta.active(False)
    if not ap.active():
        print("AP is not active, activating....")
        ap.active(True)
    ap.config(essid=ssid, hidden=False, authmode=3, password=password)
    machine.reset()
