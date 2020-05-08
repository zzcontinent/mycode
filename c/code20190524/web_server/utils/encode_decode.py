# -*- coding: utf-8 -*-
# Created by coder-zhuyu on 2018/8/21
"""
"""
import pyDes
import binascii
import traceback
import sys

key = 'xxxxxxx'


def enc(data, ikey=key):
    k = pyDes.des(ikey, pyDes.CBC, "\0\0\0\0\0\0\0\0", pad=None, padmode=pyDes.PAD_PKCS5)
    d = k.encrypt(data)
    return bytes.decode(binascii.hexlify(d))


def dec(cipher_text, ikey=key):
    k = pyDes.des(ikey, pyDes.CBC, "\0\0\0\0\0\0\0\0", pad=None, padmode=pyDes.PAD_PKCS5)
    return bytes.decode(k.decrypt(binascii.unhexlify(cipher_text)))


#
if __name__ == '__main__':
    try:
        print(enc(sys.argv[1], key))
        print(dec(sys.argv[1], key))
        pass
    except Exception as e:
        print(e)
        print(traceback.format_exc())
    finally:
        pass
