import requests
import json

postdata = json.dumps({
    "username": "cliff",
    "password": "123456"
}, ensure_ascii=False)

# success test demo
r = requests.post("http://localhost:9090/auth/login", data=postdata)
print(r.text)

r = requests.get("http://localhost:9090/auth/logout", data=postdata)
print(r.text)

# fail test demo
postdata = json.dumps({
    "username": "cliff",
    "password": "xxxx"
}, ensure_ascii=False)
r = requests.post("http://localhost:9090/auth/login", data=postdata)
print(r.text)

r = requests.post("http://localhost:9090/auth/logout", data=postdata)
print(r.text)

'''
server stdout:
[D 190526 12:44:05 __init__:15] app.api_1_0
[D 190526 12:44:05 __init__:20] app.api_1_0.auth_handler
[('/auth/login', <class 'app.api_1_0.auth_handler.AuthLoginHandler'>), ('/auth/logout', <class 'app.api_1_0.auth_handler.AuthLogoutHandler'>)]
[I 190526 12:44:15 web:2162] 200 POST /auth/login (127.0.0.1) 0.79ms
[I 190526 12:45:00 web:2162] 200 POST /auth/login (127.0.0.1) 0.52ms
[I 190526 12:45:00 web:2162] 200 GET /auth/logout (127.0.0.1) 0.59ms
[I 190526 12:46:02 web:2162] 200 POST /auth/login (127.0.0.1) 0.54ms
[I 190526 12:46:02 web:2162] 200 GET /auth/logout (127.0.0.1) 0.53ms
[I 190526 12:46:02 web:2162] 200 POST /auth/login (127.0.0.1) 0.44ms
[I 190526 12:46:02 web:2162] 200 GET /auth/logout (127.0.0.1) 0.41ms
[I 190526 12:46:09 web:2162] 200 POST /auth/login (127.0.0.1) 0.65ms
[I 190526 12:46:09 web:2162] 200 GET /auth/logout (127.0.0.1) 0.45ms
[I 190526 12:46:09 web:2162] 200 POST /auth/login (127.0.0.1) 0.46ms
[W 190526 12:46:09 web:2162] 405 POST /auth/logout (127.0.0.1) 0.38ms
[I 190526 12:47:07 web:2162] 200 POST /auth/login (127.0.0.1) 0.58ms
[I 190526 12:47:07 web:2162] 200 GET /auth/logout (127.0.0.1) 0.53ms
[I 190526 12:47:07 web:2162] 200 POST /auth/login (127.0.0.1) 0.49ms
[W 190526 12:47:07 web:2162] 405 POST /auth/logout (127.0.0.1) 0.38ms

client stdout:
{"code": 0, "msg": "成功", "data": {"uid": 65535, "exp": 1558881969, "at": 1558845969}}
{"code": 0, "msg": "成功", "data": {"is_ok": 1, "say": "see you !"}}
{"code": 401001, "msg": "用户不存在或密码错误", "data": {}}
<html><title>405: Method Not Allowed</title><body>405: Method Not Allowed</body></html>
'''
