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
        state = req.body().readline()
        response = light_controller.led(state, context=req.context)
        w.write(response)
    return handler