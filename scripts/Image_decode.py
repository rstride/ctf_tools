#!/usr/bin/python
# -*- coding: UTF-8 -*-

import re
import base64
import urllib.request, urllib.parse
import http.cookiejar
from io import BytesIO
from PIL import Image
from PIL import ImageDraw
import zxing
import os

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
CHARSET = 'utf-8'
ROOTME_URL = 'http://challenge01.root-me.org/programmation/ch7/'


def main() :

    print('Init cookies ...')
    init_cookies()

    print('Get QR-Code image datas ...')
    image_byte = download_image()

    print('Fix QR-Code image ...')
    qrcode_path = fix_image(image_byte)

    print('Recognize ...')
    key = recognize(qrcode_path)
    print(' => %s' % key)

    print('Sumbit key ...')
    password = submit(key)
    print(' => %s' % password)

    print('Show Image ...')
    image = Image.open(qrcode_path)
    image.show()
    os.remove(qrcode_path)



def init_cookies() :

    cj = http.cookiejar.CookieJar()
    opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cj))
    urllib.request.install_opener(opener)
    return


def download_image() :

    lines = urllib.request.urlopen(ROOTME_URL).readlines()
    html = lines[0].decode(CHARSET)

    pattern = re.compile(r'base64,([^"]+)')
    mth = pattern.search(html)
    image_data = mth.group(1)
    image_byte = base64.b64decode(image_data)    
    return image_byte


def fix_image(image_byte) :

    image = Image.open(BytesIO(image_byte))
    draw = ImageDraw.Draw(image)

    w = 9       
    w2 = w * 2
    w5 = w * 5
    w6 = w * 6
    w7 = w * 7
    for x, y in [(18, 18), (18, 216), (216, 18)] :
        draw.rectangle([(x, y), (x + w7, y + w7)], fill = BLACK)           
        draw.rectangle([(x + w, y + w), (x + w6, y + w6)], fill = WHITE)   
        draw.rectangle([(x + w2, y + w2), (x + w5, y + w5)], fill = BLACK) 

    TMP_QRCODE_PATH = './tmp_qrcode.png'
    image.save(TMP_QRCODE_PATH, format='PNG')
    return TMP_QRCODE_PATH


def recognize(qrcode_path) :

    reader = zxing.BarCodeReader()
    qrcode = reader.decode(qrcode_path)
    content = qrcode.parsed
    key = re.sub(re.compile(r'^The key is '), '', content)
    return key


def submit(key) :

    params = urllib.parse.urlencode({ 'metu' : key })
    post_data = bytes(params, CHARSET)
    lines = urllib.request.urlopen(ROOTME_URL, post_data).readlines()
    html = lines[0].decode(CHARSET)

    mth = re.match(r'.*?Congratz, le flag est (\w+).*$', html)
    password = ('Success: %s' % mth.group(1)) if mth else 'Error or Timeout'
    return password



if __name__ == '__main__' :
    main()

