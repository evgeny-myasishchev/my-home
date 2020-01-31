import uhttp
import logger
import sys

def use(handler, *middleware):
    pass

def not_found(next):
    return lambda writer,req: writer.write_header(uhttp.HTTP_STATUS_NOT_FOUND)

def recover(next, *, log=logger.log, debug=False):
    def middleware(writer, req):
        try:
            next(writer, req)
        except BaseException as err:
            # TODO: Also log and print stack trace
            message = uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR] if debug == False else str(err)
            writer.write_header(uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR)
            writer.write(message)

    return middleware