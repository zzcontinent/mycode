# -*- coding: utf-8 -*-
# Created by coder-zhuyu on 2018/9/7
"""
"""
from config import config

settings = dict(
    debug=config.debug
)


class Route(object):
    """ 把每个URL与Handler的关系保存到一个元组中，然后追加到列表内，列表内包含了所有的Handler """

    def __init__(self):
        self.urls = list()  # 路由列表

    def __call__(self, url, *args, **kwargs):
        def register(cls):
            self.urls.append((url, cls))  # 把路由的对应关系表添加到路由列表中
            return cls

        return register


route = Route()  # 创建路由表对象
