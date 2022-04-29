#!/usr/bin/env python3
# Import Basic OS functions
import os
# Import modules for CGI handling
import cgi, cgitb
import urllib.request

# enable debugging
cgitb.enable()
# print content type
print("Content-type:text/html\r\n\r\n")

form = cgi.FieldStorage()

UPLOAD_DIR='../upload/'

# IF A FILE WAS UPLOADED (name=file) we can find it here.
if "file" in form:
	form_file = form['file']
	# form_file is now a file object in python
	if form_file.filename:
		uploaded_file_path = os.path.join(UPLOAD_DIR, os.path.basename(form_file.filename))

		with open(uploaded_file_path, 'wb') as fout:
			# read the file in chunks as long as there is data
			while True:
				chunk = form_file.file.read(100000)
				if not chunk:
					break
				# write the file content on a file on the hdd
				fout.write(chunk)
		print("File uploaded!")
	else:
		print("Bad: Filename not found!")
else:
	print("Bad: File not found!")

