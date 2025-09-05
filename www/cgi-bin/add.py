#!/usr/bin/python
import os
import sys

# --- Script Logic ---
result = None
error_message = None
params = {}

# Get the query string from the environment variable.
query_string = os.environ.get('QUERY_STRING')

# Parse the query string into a dictionary.
if query_string:
	pairs = query_string.split('&')
	for pair in pairs:
		if '=' in pair:
			key, value = pair.split('=', 1)
			params[key] = value

# Try to perform the addition.
try:
	val_a = int(params['a'])
	val_b = int(params['b'])
	result = val_a + val_b
except (KeyError, ValueError):
	# This block runs if parameters were missing or not valid numbers.
	error_message = "Error: Please provide two numbers in the URL. Example: <strong>?a=5&b=10</strong>"

# --- HTTP Header & Output ---

# 1. Write the Content-Type header with the correct CRLF ending.
sys.stdout.write("Content-Type: text/html\r\n")

# 2. Write the mandatory blank line (which is just a CRLF).
sys.stdout.write("\r\n")

# 3. Print the HTML content normally. The print() function will use standard newlines.
print("<html>")
print("<head><title>CGI Big Number Adder</title></head>")
print('<body style="display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0;">')

# Check if an error occurred and display the appropriate message.
if error_message:
	print(f"<p>{error_message}</p>")
else:
	# If successful, display the result with custom style.
	print('<div style="font-size: 40px; font-weight: bold; color: red; text-align: center;">')
	print(result)
	print('</div>')

print("</body>")
print("</html>")