#!/usr/bin/python3
import os
import json
import uuid
import html
import sys
# import cgi # Deprecated in Python 3.11+. Use email.parser and cgi.FieldStorage alternative if needed.

try:
    import cgi
except ImportError:
    # Fallback for Python 3.11+ where cgi is removed
    from http import server
    import io
    import email
    import tempfile

    class SimpleFieldStorage:
        def __init__(self, environ, fp):
            self.form = {}
            content_type = environ.get('CONTENT_TYPE', '')
            if 'multipart/form-data' in content_type:
                pdict = {}
                if 'boundary=' in content_type:
                    pdict['boundary'] = content_type.split('boundary=')[1].encode()
                content_length = int(environ.get('CONTENT_LENGTH', 0))
                body = fp.read(content_length)
                msg = email.message_from_bytes(
                    b'Content-Type: ' + content_type.encode() + b'\n\n' + body
                )
                for part in msg.walk():
                    if part.get_content_maintype() == 'multipart':
                        continue
                    name = part.get_param('name', header='content-disposition')
                    filename = part.get_param('filename', header='content-disposition')
                    if filename:
                        self.form[name] = {'filename': filename, 'file': io.BytesIO(part.get_payload(decode=True))}
                    else:
                        self.form[name] = part.get_payload(decode=True).decode()
            else:
                # Handle urlencoded
                qs = fp.read(int(environ.get('CONTENT_LENGTH', 0))).decode()
                for kv in qs.split('&'):
                    if '=' in kv:
                        k, v = kv.split('=', 1)
                        self.form[k] = v

        def getvalue(self, key, default=None):
            val = self.form.get(key, default)
            if isinstance(val, dict) and 'file' in val:
                return val['file']
            return val

        def __getitem__(self, key):
            return self.form[key]

# Use cgi.FieldStorage or fallback
def get_form():
    try:
        return cgi.FieldStorage()
    except NameError:
        return SimpleFieldStorage(os.environ, sys.stdin.buffer)
import urllib.parse

# --- CONFIGURATION (Portable) ---

# The full path to the directory containing this script (e.g., /home/fdi-cecc/webserv/www/cgi-bin)
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

# Go up two levels from /cgi-bin to the project's www root
WWW_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, '..'))

# Define paths based on the www root
IMAGE_DIR = os.path.join(WWW_ROOT, "uploads", "gallery_images")
DATA_FILE = os.path.join(WWW_ROOT, "uploads", "gallery_data.json")
# The relative path from the HTML to the images
IMAGE_WEB_PATH = "/uploads/gallery_images/"

def handle_delete(params):
    """Deletes a pet's image and its entry from the JSON data file."""
    image_id_to_delete = params.get("id", [None])[0]
    if not image_id_to_delete:
        return

    # Load existing data
    try:
        with open(DATA_FILE, 'r') as f:
            pets = json.load(f)
    except (IOError, json.JSONDecodeError):
        pets = []

    # Find and remove the pet entry
    pets_to_keep = [pet for pet in pets if pet['image'] != image_id_to_delete]
    
    # If a pet was removed, delete its image file
    if len(pets_to_keep) < len(pets):
        try:
            file_path = os.path.join(IMAGE_DIR, image_id_to_delete)
            if os.path.exists(file_path):
                os.remove(file_path)
        except OSError:
            pass # Ignore errors during file deletion
    
    # Save the updated list back to the JSON file
    with open(DATA_FILE, 'w') as f:
        json.dump(pets_to_keep, f, indent=4)


def handle_post():
    """Processes the uploaded form data, saves the file, and updates the JSON."""
    form = cgi.FieldStorage()
    
    pet_name = form.getvalue("pet_name", "Unnamed Pet")
    picture_item = form["pet_picture"]

    if picture_item.filename:
        extension = os.path.splitext(picture_item.filename)[1]
        unique_filename = f"{uuid.uuid4()}{extension}"
        
        file_path = os.path.join(IMAGE_DIR, unique_filename)
        with open(file_path, 'wb') as f:
            f.write(picture_item.file.read())

        try:
            with open(DATA_FILE, 'r') as f:
                pets = json.load(f)
        except (IOError, json.JSONDecodeError):
            pets = []
        
        pets.append({"name": pet_name, "image": unique_filename})
        
        with open(DATA_FILE, 'w') as f:
            json.dump(pets, f, indent=4)

    # --- Post/Redirect/Get Pattern ---
    script_url = os.environ.get("SCRIPT_NAME", "")
    print(f"Status: 303 See Other")
    print(f"Location: {script_url}")
    print()

def handle_get():
    """Displays the gallery and the upload form, and handles delete actions."""
    query_string = os.environ.get("QUERY_STRING", "")
    params = urllib.parse.parse_qs(query_string)

    # --- NEW: Check if the action is 'delete' ---
    if params.get("action", [None])[0] == 'delete':
        handle_delete(params)
        # Redirect back to the clean gallery page after deletion
        script_url = os.environ.get("SCRIPT_NAME", "")
        print(f"Status: 303 See Other")
        print(f"Location: {script_url}")
        print()
        return # Stop execution to complete the redirect

    # --- If not deleting, proceed to display the page ---
    try:
        with open(DATA_FILE, 'r') as f:
            pets = json.load(f)
    except (IOError, json.JSONDecodeError):
        pets = []

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
            .card {{ background-color: #fff; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); overflow: hidden; width: 250px; text-align: center; padding-bottom: 10px; }}
            .card img {{ width: 100%; height: 200px; object-fit: cover; }}
            .card h3 {{ margin: 15px 0; }}
            .delete-link {{ color: #e74c3c; text-decoration: none; font-size: 14px; }}
            .delete-link:hover {{ text-decoration: underline; }}
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

    if not pets:
        print("<p>No pets uploaded yet. Be the first!</p>")
    else:
        for pet in reversed(pets):
            safe_name = html.escape(pet['name'])
            image_url = os.path.join(IMAGE_WEB_PATH, pet['image'])
            image_id = pet['image']
            # --- NEW: Added the delete link ---
            delete_url = f"{script_url}?action=delete&id={image_id}"
            print(f"""
            <div class="card">
                <img src="{image_url}" alt="{safe_name}">
                <h3>{safe_name}</h3>
                <a href="{delete_url}" class="delete-link">Delete</a>
            </div>
            """)

    print(f"""
        </div>
        <hr>
        <h2>Upload a New Pet</h2>
        <div class="upload-form">
            <form action="{script_url}" method="post" enctype="multipart/form-data">
                <label for="pet_name">Pet's Name:</label>
                <input type="text" id="pet_name" name="pet_name" required>
                <label for="pet_picture">Picture:</label>
                <input type="file" id="pet_picture" name="pet_picture" accept="image/*" required>
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