#!/usr/bin/env python3

import cgi, cgitb
import time

cgitb.enable()

input_data = cgi.FieldStorage()

while(1):
	time.sleep(1)
