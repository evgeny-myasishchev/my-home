import logger

class ATException(Exception):
    def __init__(self, message='AT command failed', data=None):
        super().__init__(message)
        self.data = data
        pass

class ATEngine:
    def __init__(self, input, output):
        self._resp_loop_limit = 10
        self._input = input
        self._output = output
        pass

    def dispatch_command(self, cmd, data=None, *, context=None):
        logger.debug('Dispatching AT command: AT+%s' % cmd, context=context, data=data)
        self._output.write('AT+')
        self._output.write(cmd)
        if data != None:
            self._output.write('=')
            self._output.write(data)
        self._output.write('\n')
        status = None
        status_message = None
        data = []
        attempts_made = 0
        while True:
            attempts_made = attempts_made + 1
            if attempts_made >= self._resp_loop_limit:
                status = 'ERROR'
                status_message = 'Response loop limit reached'
                break
            raw_resp_line = self._input.readline()
            if raw_resp_line == None:
                continue
            resp_line = str(raw_resp_line, 'utf-8').strip()
            if (resp_line == 'OK') or (resp_line == 'ERROR'):
                status = resp_line
                break
            data.append(resp_line)
        logger.debug('AT command dispatched, got status: %s' % status, data={
            'status_message': status_message,
            'response': data,
        }, context=context)
        if status != 'OK':
            raise ATException(status_message, data=data)
        return data