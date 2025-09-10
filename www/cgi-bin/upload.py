#!/usr/bin/env python3
import os
import sys
import html

# --- CONFIGURATION ---
# The directory where files will be saved.
# IMPORTANT: This path is relative to where the script is executed from.
# Make sure it exists and the web server has permission to write to it.
UPLOAD_DIR = './www/uploads/'

def send_response(status="200 OK", content_type="text/html", body=""):
    """Prints a complete and valid HTTP response to standard output."""
    sys.stdout.write(f"Status: {status}\r\n")
    sys.stdout.write(f"Content-Type: {content_type}\r\n")
    sys.stdout.write(f"Content-Length: {len(body)}\r\n")
    sys.stdout.write("\r\n") # End of headers
    sys.stdout.write(body)

def parse_multipart_form_data():
    """
    Parses a multipart/form-data POST request from stdin without using the cgi module.
    Returns a dictionary with form fields, where file uploads are stored as a
    tuple: (filename, file_content_bytes).
    """
    try:
        content_type = os.environ['CONTENT_TYPE']
        content_length = int(os.environ['CONTENT_LENGTH'])
        boundary = '--' + content_type.split('boundary=')[1]
    except (KeyError, IndexError, ValueError):
        return None, "Missing or invalid CONTENT_TYPE or CONTENT_LENGTH headers."

    # Read the entire request body from standard input as bytes
    body_bytes = sys.stdin.buffer.read(content_length)
    
    parts = body_bytes.split(boundary.encode())
    form_data = {}

    for part in parts:
        if b'Content-Disposition' not in part:
            continue

        try:
            # Each part has headers and a body, separated by a double newline
            headers_raw, content = part.split(b'\r\n\r\n', 1)
            # The content ends with a newline, which we strip
            content = content.rstrip(b'\r\n')
            
            # Parse the part's headers
            headers = headers_raw.decode().split('\r\n')
            disposition = next((h for h in headers if 'Content-Disposition' in h), None)
            
            if disposition:
                # Extract field name and filename
                field_name = ""
                filename = ""
                disp_parts = disposition.split(';')
                for item in disp_parts:
                    item = item.strip()
                    if item.startswith('name='):
                        field_name = item.split('=')[1].strip('"')
                    elif item.startswith('filename='):
                        filename = item.split('=')[1].strip('"')
                
                if field_name:
                    if filename:
                        # This is a file upload
                        form_data[field_name] = (filename, content)
                    else:
                        # This is a regular form field
                        form_data[field_name] = content.decode()
        except Exception:
            # Ignore malformed parts
            continue
            
    return form_data, None


def main():
    """Main script logic."""
    if os.environ.get("REQUEST_METHOD") != "POST":
        send_response("405 Method Not Allowed", body="<html><body><h1>405 Method Not Allowed</h1></body></html>")
        return

    # Ensure the upload directory exists
    try:
        os.makedirs(UPLOAD_DIR, exist_ok=True)
    except OSError as e:
        error_html = f"<html><body><h1>Server Configuration Error</h1><p>Could not create upload directory: {html.escape(str(e))}</p></body></html>"
        send_response("500 Internal Server Error", body=error_html)
        return

    form, error = parse_multipart_form_data()

    if error:
        error_html = f"<html><body><h1>Bad Request</h1><p>Could not parse form data: {html.escape(error)}</p></body></html>"
        send_response("400 Bad Request", body=error_html)
        return
        
    if 'filename' in form and isinstance(form['filename'], tuple):
        original_filename, file_bytes = form['filename']

        if original_filename:
            # Security: Use only the base name of the file to prevent path traversal attacks
            safe_filename = os.path.basename(original_filename)
            filepath = os.path.join(UPLOAD_DIR, safe_filename)
            
            try:
                with open(filepath, 'wb') as f:
                    f.write(file_bytes)
                message = f'The file "{html.escape(safe_filename)}" was uploaded successfully.'
                body = f"<html><body><h1>Upload Success!</h1><p>{message}</p></body></html>"
                send_response("200 OK", body=body)
            except IOError as e:
                message = f'Could not save file. Error: {html.escape(str(e))}'
                body = f"<html><body><h1>Error</h1><p>{message}</p></body></html>"
                send_response("500 Internal Server Error", body=body)
        else:
            message = 'No file was selected in the form.'
            body = f"<html><body><h1>Upload Status</h1><p>{message}</p></body></html>"
            send_response("400 Bad Request", body=body)
    else:
        message = 'The form is missing the required "filename" field for the file upload.'
        body = f"<html><body><h1>Bad Request</h1><p>{message}</p></body></html>"
        send_response("400 Bad Request", body=body)

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        # Catch-all for any unexpected errors during script execution
        body = f"<html><body><h1>500 Internal Server Error</h1><p>An unexpected error occurred in the Python script: {html.escape(str(e))}</p></body></html>"
        send_response("500 Internal Server Error", body=body)
