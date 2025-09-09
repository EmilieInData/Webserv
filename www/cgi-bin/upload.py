#!/usr/bin/python3
import cgi
import os

# The directory where files will be saved (make sure it exists and has write permissions)
UPLOAD_DIR = '../uploads/'

form = cgi.FieldStorage()

status_line = "Status: 200 OK"
headers = ["Content-Type: text/html"]
body = ""

if 'filename' in form:
    fileitem = form['filename']

    if fileitem.filename:
        fn = os.path.basename(fileitem.filename)
        filepath = os.path.join(UPLOAD_DIR, fn)
        
        try:
            with open(filepath, 'wb') as f:
                f.write(fileitem.file.read())
            message = f'The file "{fn}" was uploaded successfully to {filepath}'
            body = f"<html><body><h1>Success!</h1><p>{message}</p></body></html>"
        except IOError as e:
            status_line = "Status: 500 Internal Server Error"
            message = f'Could not save file. Error: {e}'
            body = f"<html><body><h1>Error</h1><p>{message}</p></body></html>"
    else:
        message = 'No file was uploaded.'
        body = f"<html><body><h1>Upload Status</h1><p>{message}</p></body></html>"
else:
    message = 'Form is missing the "filename" field.'
    body = f"<html><body><h1>Error</h1><p>{message}</p></body></html>"

print(status_line)
for header in headers:
    print(header)
print() 
print(body)