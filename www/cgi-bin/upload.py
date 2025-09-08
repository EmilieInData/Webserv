#!/usr/bin/python3
import sys
import os

# The location for our log file. Ensure this directory is writable by the server.
LOG_FILE = './cgi_debug.log' 

try:
    # Open the log file for writing
    with open(LOG_FILE, 'w') as f:
        f.write("--- CGI Debug Log ---\n\n")
        
        # Log all environment variables given by the server
        f.write("## Environment Variables:\n")
        for key, value in os.environ.items():
            f.write(f"{key}: {value}\n")
        
        f.write("\n---\n\n")
        
        # Check for CONTENT_LENGTH to know how much data to read
        content_length_str = os.environ.get('CONTENT_LENGTH', '0')
        f.write(f"## STDIN Content (based on CONTENT_LENGTH={content_length_str}):\n")

        if content_length_str:
            content_length = int(content_length_str)
            # Read the exact number of bytes from standard input
            stdin_body = sys.stdin.read(content_length)
            f.write(stdin_body)
        else:
            f.write("No CONTENT_LENGTH, so stdin was not read.\n")

    # Send a success message to the browser
    print("Content-Type: text/html\r\n\r\n")
    print("<html><body><h1>Debug log created</h1>")
    print(f"<p>Check the server for the file: <b>{os.path.abspath(LOG_FILE)}</b></p></body></html>")

except Exception as e:
    # If an error happens, try to report it to the browser
    print("Content-Type: text/html\r\n\r\n")
    print(f"<html><body><h1>CGI Script Error</h1><p>{e}</p></body></html>")