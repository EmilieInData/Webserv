#!/usr/bin/python3
import os
import sys
import urllib.parse

def handle_post():
    """Handle POST request data"""
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    content_type = os.environ.get('CONTENT_TYPE', '')
    
    if content_length == 0:
        return {}
    
    # Read the POST data
    post_data = sys.stdin.read(content_length)
    
    # Parse different content types
    if 'application/x-www-form-urlencoded' in content_type:
        # Standard form data
        return urllib.parse.parse_qs(post_data)
    elif 'application/json' in content_type:
        # JSON data (you'd need to import json and parse it)
        return {'raw_json': post_data}
    else:
        # Raw data
        return {'raw_data': post_data}

def main():
    method = os.environ.get('REQUEST_METHOD', 'GET')
    
    print("Content-Type: text/html")
    print()  # End of headers
    
    print("""<!DOCTYPE html>
<html>
<head>
    <title>Simple POST Handler</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .form-container { max-width: 500px; }
        input, textarea { width: 100%; padding: 8px; margin: 5px 0; }
        button { background: #007cba; color: white; padding: 10px 20px; border: none; cursor: pointer; }
        .result { background: #f0f0f0; padding: 15px; margin: 20px 0; border-radius: 5px; }
    </style>
</head>
<body>
    <h1>Simple POST Handler</h1>""")
    
    if method == 'POST':
        # Handle POST request
        post_data = handle_post()
        
        print('<div class="result">')
        print('<h3>POST Data Received:</h3>')
        
        if post_data:
            for key, value in post_data.items():
                if isinstance(value, list):
                    # URL-encoded form data comes as lists
                    value = value[0] if value else ''
                print(f'<p><strong>{key}:</strong> {value}</p>')
        else:
            print('<p>No POST data received</p>')
        
        print('</div>')
    
    # Always show the form
    script_name = os.environ.get('SCRIPT_NAME', '')
    print(f"""
    <div class="form-container">
        <h3>Send a POST Request:</h3>
        <form action="{script_name}" method="post">
            <label>Name:</label>
            <input type="text" name="name" required>
            
            <label>Email:</label>
            <input type="email" name="email" required>
            
            <label>Message:</label>
            <textarea name="message" rows="4" required></textarea>
            
            <button type="submit">Submit POST Request</button>
        </form>
    </div>
    
    <hr>
    
    <h3>Environment Info:</h3>
    <p><strong>Request Method:</strong> {method}</p>
    <p><strong>Content Type:</strong> {os.environ.get('CONTENT_TYPE', 'None')}</p>
    <p><strong>Content Length:</strong> {os.environ.get('CONTENT_LENGTH', '0')}</p>
    <p><strong>Query String:</strong> {os.environ.get('QUERY_STRING', 'None')}</p>
    
</body>
</html>""")

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("Content-Type: text/html")
        print()
        print(f"<h1>Error</h1><p>{str(e)}</p>")