# -*- coding: utf-8 -*-


class ValidationError(ValueError):
    pass


class UserIDException(Exception):
    def __init__(self):
        Exception.__init__(self, 'user_id not equal')


class NoDBConnectConfigException(Exception):
    def __init__(self):
        Exception.__init__(self, 'no db conn_name config')
