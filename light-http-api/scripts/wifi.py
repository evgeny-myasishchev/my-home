import sys
# import machine

# ap = network.WLAN(network.AP_IF)
# ap.active(False)
# sta = network.WLAN(network.STA_IF)
# sta.active(True)
# sta.connect("$(WIFI_SSID)", "$(WIFI_PASSWORD)")
# machine.reset()

args = {
    "cmd": "help",
    "ssid": "",
    "password": ""
}

argI = 1
argv = sys.argv
while argI < len(sys.argv):
    argName = argv[argI].replace("--", "")
    if argName in args:
        argI += 1
        args[argName] = argv[argI]
    else:
        print("Unexpected arg:", argName)
        sys.exit(1)
    argI += 1

def showHelp(args):
    print("Wifi management. Commands: setup-ap, setup-sta, status, help")
    print("Args: --ssid <ssid>, --password <password> (for setup commands only)")

commands = {
    "help": showHelp
}
cmd = args["cmd"]
if cmd in commands:
    fn = commands[cmd]
    fn(args)