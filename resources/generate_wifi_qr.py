#!/usr/bin/env python3

import segno
from segno import helpers
wifi_qrcode = helpers.make_wifi_data(ssid="ESP32-Access-Point", password="123456789", security="WPA")
wifi_qrcode = segno.make(wifi_qrcode)
wifi_qrcode.save("wifi.svg", scale=4)
wifi_qrcode.save("wifi.png", scale=4)

url_qrcode = segno.make("http://http://192.168.4.1:80")
url_qrcode.save("url.svg", scale=4)
url_qrcode.save("url.png", scale=4)