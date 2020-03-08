import sys
import uio
import ujson

def load():
    config_path = "config/platform-%s.json" % sys.platform
    config_stream = None
    try:
        config_stream = uio.open(config_path)
        return ujson.load(config_stream)
    except OSError as err: 
        raise Exception("Failed to open config file: %s - %s" % (config_path, err))
    finally:
        if config_stream != None:
            config_stream.close()
