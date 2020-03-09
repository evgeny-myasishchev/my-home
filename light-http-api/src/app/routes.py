def create_ping_handler(light_controller):
    def handler(w, req):
        response = light_controller.ping(context=req.context)
        w.write(response)
    return handler

def create_led_handler(light_controller):
    def handler(w, req):
        state = req.body().readline()
        response = light_controller.led(state, context=req.context)
        w.write(response)
    return handler