# -*- coding: utf-8 -*-
import tornado.options
import tornado.httpserver
import tornado.ioloop
import asyncio

from tornado.platform.asyncio import AsyncIOMainLoop
from tornado.options import define, options
from dotenv import load_dotenv, find_dotenv

# load environment
load_dotenv(find_dotenv())

from app import create_app

define("host", default="0.0.0.0", help="run on the given host", type=str)
define("port", default=9090, help="run on the given port", type=int)


def main():
    AsyncIOMainLoop().install()

    tornado.options.parse_command_line()

    loop = asyncio.get_event_loop()

    application = create_app()
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(options.port, options.host)

    loop.run_forever().start()


if __name__ == '__main__':
    main()
