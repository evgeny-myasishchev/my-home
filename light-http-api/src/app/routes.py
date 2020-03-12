import ujson
import uhttp
import logger

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
        state = payload['state']
        logger.debug('Set led state to: %s' % state, context=req.context)
        light_controller.led(state, context=req.context)
        w.write_header(uhttp.HTTP_STATUS_NO_CONTENT)
    return handler

def create_get_pin_handler(light_controller):
    def handler(w, req, match):
        pinNumber = match.group(1)
        response = light_controller.get_pin(pinNumber, context=req.context)
        w.write(ujson.dumps({
            'state': response
        }))
    return handler

def create_set_pin_handler(light_controller):
    def handler(w, req, match):
        pinNumber = int(match.group(1))
        payload = ujson.load(req.body())
        state = payload['state']
        logger.debug('Set pin %d state to: %s' % (pinNumber, state), context=req.context)
        light_controller.set_pin(pinNumber, state, context=req.context)
        w.write_header(uhttp.HTTP_STATUS_NO_CONTENT)
    return handler
