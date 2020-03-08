def create_ping_handler():
    def handler(w, req):
        w.write("PONG")
    return handler