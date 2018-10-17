#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import msgpack
import socket

BUFFER_SIZE = 4096

connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

connection.connect((ip, port))
connection.bind(("127.0.0.1", port))

connection.send('GET /index.html HTTP/1.1\r\nHost: {}.{}\r\n\r\n'.format(sld, tld))

response = connection.recv(BUFFER_SIZE)
