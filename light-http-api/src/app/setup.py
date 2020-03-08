import logger

def logger_transport(log_config):
    transports = []
    transports_config = log_config["transports"]
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