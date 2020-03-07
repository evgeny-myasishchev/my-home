import uhttp
import logger
import sys
from uuid import uuid4
import gc
import uerrno

def use(handler, *middleware):
    next = handler
    for mw in reversed(middleware):
        next = mw(next)
    return next

def not_found(next):
    return lambda writer,req: writer.write_header(uhttp.HTTP_STATUS_NOT_FOUND)

def recover(next, *, debug=False, logger=logger):
    def middleware(writer, req):
        err = None
        status = uhttp.HTTP_STATUS_INTERNAL_SERVER_ERROR
        try:
            next(writer, req)
        except OSError as e:
            if e.args[0] == uerrno.ETIMEDOUT:
                status = uhttp.HTTP_STATUS_REQUEST_TIMEOUT
                pass
            err = e
        except BaseException as e:
            err = e
        if err != None:
            message = uhttp.HTTP_REASON_PHRASE[status] if debug == False else str(err)
            writer.write_header(status)
            writer.write(message)
            logger.error("Failed to handle request", context=req.context, err=err)
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

def create_router(routes):
    def router(next):
        def middleware(w, req):
            for route in routes:
                method = route[0]
                pattern = route[1]
                handler = route[2]
                if req.method == method:
                    if hasattr(pattern, "match"):
                        match = pattern.match(req.uri)
                        if match != None:
                            handler(w, req, match)
                            return
                    elif req.uri.startswith(pattern):
                        handler(w, req)
                        return
            next(w, req)
        return middleware
    return router
