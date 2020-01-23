import sys

def hex_string(val):
    return '%X' % val

def status():
    import network
    sta = network.WLAN(network.STA_IF)
    ap = network.WLAN(network.AP_IF)
    if sta.active(): 
        print("STA active")
        print("ifconfig:", sta.ifconfig())
        print("essid:", sta.config('essid'))
        print("mac:", ':'.join(map(hex_string, sta.config('mac'))))
    if ap.active():
        print("AP active")
        print("ifconfig:", ap.ifconfig())
        print("essid:", ap.config('essid'))
        print("authmode:", ap.config('authmode'))

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
