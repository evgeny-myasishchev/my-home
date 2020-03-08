def create_ping_handler(light_controller):
    def handler(w, req):
        response = light_controller.ping(context=req.context)
        w.write(response)
    return handler