import ujson

def create_ping_handler(light_controller):
    ping_payload = 'PONG'
    def handler(w, req):
        response = light_controller.ping(ping_payload, context=req.context)
        w.write(ujson.dumps({
            'controller': response,
            'service': ping_payload,
        }))
    return handler

def create_led_handler(light_controller):
    def handler(w, req):
        payload = ujson.load(req.body())
        response = light_controller.led(payload['state'], context=req.context)
        w.write(response)
    return handler
