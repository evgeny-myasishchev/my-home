import uhttp
import logger
import sys
from uuid import uuid4
import gc

def use(handler, *middleware):
    next = handler
    for mw in reversed(middleware):
        next = mw(next)
    return next

def not_found(next):
    return lambda writer,req: writer.write_header(uhttp.HTTP_STATUS_NOT_FOUND)

def recover(next, *, debug=False, logger=logger):
    def middleware(writer, req):
        try:
            next(writer, req)
        except BaseException as err:
            # TODO: Also log and print stack trace
            message = uhttp.HTTP_REASON_PHRASE[uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR] if debug == False else str(err)
            writer.write_header(uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR)
            writer.write(message)
    return middleware

def trace(next, *, logger=logger, uuid_fn=uuid4, gc=gc):
    def middleware(writer, req):
        if "x-request-id" in req.headers:
            req.context['requestId'] = req.headers['x-request-id']
        else:
            req.context['requestId'] = str(uuid_fn())
        logger.info(
            "BEGIN REQ: %s %s" % (req.method, req.uri),
            context=req.context,
            data={
                "method": req.method,
                "host": req.headers["host"],
                "uri": req.uri,
                "userAgent": req.headers["user-agent"],
                "headers": req.headers,
                "memoryUsage": "(mem_alloc: %s, mem_free: %s)" % (gc.mem_alloc(), gc.mem_free()),
            }
        )
        next(writer, req)
        logger.info(
            "END REQ: %s" % writer.status, 
            context=req.context,
            data={
                "status": writer.status,
                "headers": writer.headers,
                "memoryUsage": "(mem_alloc: %s, mem_free: %s)" % (gc.mem_alloc(), gc.mem_free()),
            }
        )
    return middleware