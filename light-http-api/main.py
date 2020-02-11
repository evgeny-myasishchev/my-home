import app
import logger
import uio
import sys
import ujson

def load_config():
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

def setup_logger_transport(config):
    transports = []
    transports_config = config["log"]["transports"]
    if transports_config["print"]["enabled"]:
        transports.append(logger.print_transport)
    if transports_config["file"]["enabled"]:
        file_cfg = transports_config["file"]
        transports.append(logger.make_file_transport(file_cfg["path"]))
    if transports_config["udp"]["enabled"]:
        udp_cfg = transports_config["udp"]
        transports.append(logger.make_udp_transport(
            udp_cfg["host"], 
            udp_cfg["port"],
            broadcast=udp_cfg["broadcast"]
        ))
    return logger.make_combined_transport(*transports)

config = load_config()
logger.setup(transport=setup_logger_transport(config))

server = None
try:
    server = app.start_server(
        port=config["server"]["port"]
    )
except KeyboardInterrupt as err:
    logger.warn("Keyboard interrupt ignored", err=err)
except BaseException as err:
    logger.error("Failed to start server\n", err=err)