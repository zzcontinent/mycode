# -*- coding: utf-8 -*-
from tornado.web import RequestHandler
import json
from .exceptions import *
from .codemsg import CodeMsg

class BaseRequestHandler(RequestHandler):
    """Base class for HTTP request handlers.

    Override the method get_current_user in coroutine.
    Add a method response_json for HTTP json response.
    """

    async def prepare(self):
        """Called at the beginning of a request before  `get`/`post`/etc."""
        self.__body_data = self.request.body.decode('utf8')
        try:
            self.__body_data_dict = json.loads(self.__body_data)
        except Exception as e:
            self.__body_data_dict = {}

    def get_json_data(self, key):
        return self.__body_data_dict.get(key, None)

    def response_json(self, data=None, code=0, status=200):
        """HTTP response in the form of json.
        pass

        :param data: return body
        :param code: return code
        :param status: http status
        :return:
        """
        resp_dict = {
            'code': code,
            'msg': CodeMsg.get_msg(code)
        }

        if data is not None:
            resp_dict['data'] = data
        else:
            resp_dict['data'] = {}

        self.set_status(status)
        self.set_header('Content-Type', 'application/json; charset=utf-8')

        self.write(json.dumps(resp_dict, ensure_ascii=False))
