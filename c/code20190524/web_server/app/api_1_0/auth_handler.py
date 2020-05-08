# -*- coding: utf-8 -*-
from ..request_handler import BaseRequestHandler
from schema import Schema, Regex
import concurrent.futures
import bcrypt
import tornado
import tornado.escape
import tornado.web
import asyncio
import datetime
import time
from config import config
from ..route import route

# A thread pool to be used for password hashing with bcrypt.
executor = concurrent.futures.ThreadPoolExecutor(2)

root_name = 'cliff'
root_passwd = '123456'
root_id = 65535


@route('/auth/login')
class AuthLoginHandler(BaseRequestHandler):
    """Auth login."""

    async def post(self):
        user_name = self.get_json_data('username')
        password = self.get_json_data('password')

        Schema(str).validate(user_name)
        Schema(str).validate(password)

        # check password
        if password == root_passwd:
            now = datetime.datetime.now()
            data = {
                'uid': root_id,
                'exp': int(time.mktime((now + datetime.timedelta(seconds=config.token_exp_delta)).timetuple())),
                'at': int(time.mktime(now.timetuple())),
            }
            self.response_json(data=data)
        else:
            self.response_json(code=401001)


@route('/auth/logout')
class AuthLogoutHandler(BaseRequestHandler):
    """Auth logout."""

    async def get(self):
        self.response_json(data={'is_ok': 1, 'say': 'see you !'})
