#!/usr/bin/env python

import os
import cgi

# The C++ server is expected to create the session and pass the ID here.
session_id = os.environ.get('WEBSERV_SESSION_ID')
form = cgi.FieldStorage()
username = form.getvalue('username', 'guest')

# Print the HTTP headers
print("Content-Type: text/html")

# Set the cookie if the server provided a session ID
if session_id:
    print(f"Set-Cookie: session_id={session_id}; HttpOnly; Max-Age=3600") # Expires in 1 hour
    print() # End of headers
    print(f"Login successful for {username}! Your session is now active.")
else:
    print() # End of headers
    print("<h1>Login Failed</h1>")
    print("<p>Server did not provide a session ID.</p>")