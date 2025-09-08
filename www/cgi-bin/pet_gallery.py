#!/usr/bin/python3
import os
import json
import uuid
import html
import sys
import urllib.parse

# --- CONFIGURATION ---
# IMPORTANT: Update these with the correct ABSOLUTE paths for your server.
IMAGE_DIR = "/home/palmiro/42/webserv/www/uploads/gallery_images"
DATA_FILE = "/home/palmiro/42/webserv/www/uploads/gallery_data.json"
# The relative path from the HTML to the images
IMAGE_WEB_PATH = "/uploads/gallery_images/"

def parse_multipart(fp, boundary):
    """A simple parser for multipart/form-data without the cgi module."""
    parts = fp.read().split(boundary)
    form_data = {}
    for part in parts:
        if b'Content-Disposition' not in part:
            continue
        
        headers, content = part.split(b'\r\n\r\n', 1)
        headers = headers.decode()
        
        # Parse headers to find name and filename
        name_line = [h for h in headers.split('\r\n') if 'name=' in h][0]
        name = name_line.split('name="')[1].split('"')[0]
        
        if 'filename="' in name_line:
            filename = name_line.split('filename="')[1].split('"')[0]
            # Store filename and content for file fields
            form_data[name] = {'filename': filename, 'content': content.rsplit(b'\r\n', 1)[0]}
        else:
            # Store value for simple text fields
            form_data[name] = content.rsplit(b'\r\n', 1)[0].decode()
            
    return form_data

def handle_post():
    """Processes the uploaded form data, saves the file, and updates the JSON."""
    # Get boundary string from CONTENT_TYPE environment variable
    content_type = os.environ.get("CONTENT_TYPE", "")
    boundary = b'--' + content_type.split("boundary=")[1].encode()
    
    # Read raw stdin and parse the multipart data
    form = parse_multipart(sys.stdin.buffer, boundary)
    
    cat_name = form.get("cat_name", "Unnamed Cat")
    picture_item = form.get("cat_picture")

    if picture_item and picture_item['filename']:
        # Generate a unique filename to prevent overwrites
        extension = os.path.splitext(picture_item['filename'])[1]
        unique_filename = f"{uuid.uuid4()}{extension}"
        
        # Save the uploaded file
        file_path = os.path.join(IMAGE_DIR, unique_filename)
        with open(file_path, 'wb') as f:
            f.write(picture_item['content'])

        # Load existing data, add new entry, and save back to JSON
        try:
            with open(DATA_FILE, 'r') as f:
                cats = json.load(f)
        except (IOError, json.JSONDecodeError):
            cats = []
        
        cats.append({"name": cat_name, "image": unique_filename})
        
        with open(DATA_FILE, 'w') as f:
            json.dump(cats, f, indent=4)

    # --- Post/Redirect/Get Pattern ---
    script_url = os.environ.get("SCRIPT_NAME", "")
    print(f"Status: 303 See Other")
    print(f"Location: {script_url}")
    print()

def handle_get():
    """Displays the gallery and the upload form."""
    try:
        with open(DATA_FILE, 'r') as f:
            cats = json.load(f)
    except (IOError, json.JSONDecodeError):
        cats = []

    print("Content-Type: text/html")
    print("Status: 200 OK")
    print()
    
    script_url = os.environ.get("SCRIPT_NAME", "")
    
    print(f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Pet Gallery</title>
        <style>
            body {{ font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; margin: 0; padding: 20px; }}
            h1, h2 {{ text-align: center; color: #5a4a78; }}
            .gallery {{ display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; margin-bottom: 40px; }}
            .card {{ background-color: #fff; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); overflow: hidden; width: 250px; text-align: center; transition: transform 0.2s; }}
            .card:hover {{ transform: scale(1.05); }}
            .card img {{ width: 100%; height: 200px; object-fit: cover; }}
            .card h3 {{ margin: 15px 0; }}
            .upload-form {{ max-width: 500px; margin: auto; padding: 20px; background: #fff; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }}
            input[type=text], input[type=file] {{ width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; }}
            input[type=submit] {{ width: 100%; background-color: #5a4a78; color: white; padding: 14px 20px; margin: 8px 0; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; }}
            input[type=submit]:hover {{ background-color: #493b63; }}
        </style>
    </head>
    <body>
        <h1>Pet Gallery</h1>
        <div class="gallery">
    """)

    if not cats:
        print("<p>No pets uploaded yet. Be the first!</p>")
    else:
        for cat in reversed(cats):
            safe_name = html.escape(cat['name'])
            image_url = os.path.join(IMAGE_WEB_PATH, cat['image'])
            print(f"""
            <div class="card">
                <img src="{image_url}" alt="{safe_name}">
                <h3>{safe_name}</h3>
            </div>
            """)

    print(f"""
        </div>
        <hr>
        <h2>Upload a New Pet</h2>
        <div class="upload-form">
            <form action="{script_url}" method="post" enctype="multipart/form-data">
                <label for="cat_name">Pet's Name:</label>
                <input type="text" id="cat_name" name="cat_name" required>
                <label for="cat_picture">Picture:</label>
                <input type="file" id="cat_picture" name="cat_picture" accept="image/*" required>
                <input type="submit" value="Upload">
            </form>
        </div>
    </body>
    </html>
    """)

if __name__ == "__main__":
    method = os.environ.get("REQUEST_METHOD")
    if method == "POST":
        handle_post()
    else:
        handle_get()