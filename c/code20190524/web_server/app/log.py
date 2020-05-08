# -*- coding: utf-8 -*-
import logging
from logging import Formatter
from logging.handlers import RotatingFileHandler

_app_log = logging.getLogger()

debug_log = _app_log.debug
info_log = _app_log.info
warning_log = _app_log.warning
error_log = _app_log.error


def init_log(log_path, log_level):
    file_handler = RotatingFileHandler(log_path,
                                       mode='w', maxBytes=1024 * 1024 * 100,
                                       backupCount=10,
                                       encoding='utf-8')

    file_handler.setFormatter(Formatter(
        '%(asctime)s [in %(pathname)s:%(lineno)d] %(levelname)s: %(message)s '
    ))

    logging.getLogger().setLevel(log_level)
    logging.getLogger().addHandler(file_handler)
