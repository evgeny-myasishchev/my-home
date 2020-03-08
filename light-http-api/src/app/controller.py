import at

class LightController:
    def __init__(self, at_engine):
        self._at_engine = at_engine

    def ping(self, payload='PONG'):
        pass

def create_uart_interface(cfg):
    from machine import UART
    return (
        UART(cfg['input_index']), 
        UART(cfg['output_index'])
    )

def create_mock_interface(cfg):
    class mock_input:
        def __init__(self, input):
            self._input = input
            self._position = 0

        def readline(self):
            pos = self._position
            self._position += 1
            if self._position >= len(self._input):
                self._position = 0
            return (self._input[pos] + '\n').encode()

    class null_output:
        def write(self, body):
            pass

    return (
        mock_input(cfg['input']),
        null_output()
    )

def create_interface(cfg):
    type = cfg['type']
    if type == 'mock':
        return create_mock_interface(cfg[type])
    if type == 'uart':
        return create_uart_interface(cfg[type])
    raise Exception("Unexpected interface type: %s" % type)

def create_light_controller(cfg):
    interface = create_interface(cfg['light-controller']['interface'])
    at_engine = at.ATEngine(interface[0], interface[1])
    return LightController(at_engine)