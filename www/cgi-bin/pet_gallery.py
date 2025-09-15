#!/usr/bin/python3
import os
import json
import uuid
import html
import sys
import urllib.parse

# --- CONFIGURATION ---
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
WWW_ROOT = os.path.abspath(os.path.join(SCRIPT_DIR, '..'))
IMAGE_DIR = os.path.join(WWW_ROOT, "uploads", "gallery_images")
DATA_FILE = os.path.join(WWW_ROOT, "uploads", "gallery_data.json")
IMAGE_WEB_PATH = "/uploads/gallery_images/"

def ensure_directories():
    """Create necessary directories if they don't exist."""
    os.makedirs(IMAGE_DIR, exist_ok=True)
    os.makedirs(os.path.dirname(DATA_FILE), exist_ok=True)

def parse_multipart_form_data():
    """Simple multipart form data parser."""
    content_type = os.environ.get('CONTENT_TYPE', '')
    if 'multipart/form-data' not in content_type:
        return {}
    
    boundary = None
    if 'boundary=' in content_type:
        boundary = '--' + content_type.split('boundary=')[1]
    
    if not boundary:
        return {}
    
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    if content_length == 0:
        return {}
    
    # Read the entire body
    body = sys.stdin.buffer.read(content_length)
    
    form_data = {}
    parts = body.split(boundary.encode())
    
    for part in parts[1:-1]:  # Skip first empty and last boundary parts
        if not part.strip():
            continue
            
        # Split headers and content
        if b'\r\n\r\n' in part:
            headers_section, content = part.split(b'\r\n\r\n', 1)
        elif b'\n\n' in part:
            headers_section, content = part.split(b'\n\n', 1)
        else:
            continue
            
        headers = headers_section.decode('utf-8', errors='ignore')
        
        # Extract field name
        name_match = None
        filename_match = None
        for line in headers.split('\n'):
            if 'Content-Disposition:' in line and 'name=' in line:
                # Simple extraction - look for name="fieldname"
                if 'name="' in line:
                    start = line.find('name="') + 6
                    end = line.find('"', start)
                    if end > start:
                        name_match = line[start:end]
                
                if 'filename="' in line:
                    start = line.find('filename="') + 10
                    end = line.find('"', start)
                    if end > start:
                        filename_match = line[start:end]
        
        if name_match:
            # Remove trailing \r\n
            if content.endswith(b'\r\n'):
                content = content[:-2]
            elif content.endswith(b'\n'):
                content = content[:-1]
                
            if filename_match:
                # File field
                form_data[name_match] = {
                    'filename': filename_match,
                    'content': content
                }
            else:
                # Text field
                form_data[name_match] = content.decode('utf-8', errors='ignore')
    
    return form_data

def handle_delete(params):
    """Deletes a pet's image and its entry from the JSON data file."""
    image_id_to_delete = params.get("id", [None])[0]
    if not image_id_to_delete:
        return False

    try:
        with open(DATA_FILE, 'r') as f:
            pets = json.load(f)
    except (IOError, json.JSONDecodeError):
        pets = []

    pets_to_keep = [pet for pet in pets if pet['image'] != image_id_to_delete]
    
    if len(pets_to_keep) < len(pets):
        try:
            file_path = os.path.join(IMAGE_DIR, image_id_to_delete)
            if os.path.exists(file_path):
                os.remove(file_path)
        except OSError:
            pass
        
        with open(DATA_FILE, 'w') as f:
            json.dump(pets_to_keep, f, indent=4)
        return True
    
    return False

def handle_upload():
    """Processes the uploaded form data and saves the file."""
    form_data = parse_multipart_form_data()
    
    pet_name = form_data.get("pet_name", "Unnamed Pet")
    picture_data = form_data.get("pet_picture")

    if picture_data and isinstance(picture_data, dict) and picture_data.get('filename'):
        filename = picture_data['filename']
        content = picture_data['content']
        
        extension = os.path.splitext(filename)[1]
        unique_filename = f"{uuid.uuid4()}{extension}"
        
        file_path = os.path.join(IMAGE_DIR, unique_filename)
        with open(file_path, 'wb') as f:
            f.write(content)

        try:
            with open(DATA_FILE, 'r') as f:
                pets = json.load(f)
        except (IOError, json.JSONDecodeError):
            pets = []
        
        pets.append({"name": pet_name, "image": unique_filename})
        
        with open(DATA_FILE, 'w') as f:
            json.dump(pets, f, indent=4)
        
        return True, f"Successfully uploaded {pet_name}!"
    
    return False, "No valid image uploaded."

def display_gallery():
    """Displays the gallery and the upload form."""
    ensure_directories()
    
    query_string = os.environ.get("QUERY_STRING", "")
    params = urllib.parse.parse_qs(query_string)
    
    message = ""
    message_class = ""
    
    # Handle delete action
    if params.get("action", [None])[0] == 'delete':
        if handle_delete(params):
            message = "Pet deleted successfully!"
            message_class = "success"
        else:
            message = "Failed to delete pet."
            message_class = "error"
    
    # Handle upload (POST)
    if os.environ.get("REQUEST_METHOD") == "POST":
        success, msg = handle_upload()
        message = msg
        message_class = "success" if success else "error"
    
    # Load pets data
    try:
        with open(DATA_FILE, 'r') as f:
            pets = json.load(f)
    except (IOError, json.JSONDecodeError):
        pets = []

    print("Content-Type: text/html")
    print()
    
    script_url = os.environ.get("SCRIPT_NAME", "")
    
    print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Pet Gallery</title>
    <style>
        body {{ font-family: Arial, sans-serif; background-color: #f4f4f9; color: #333; margin: 0; padding: 20px; }}
        h1, h2 {{ text-align: center; color: #5a4a78; }}
        .message {{ max-width: 500px; margin: 20px auto; padding: 15px; border-radius: 5px; text-align: center; }}
        .success {{ background-color: #d4edda; color: #155724; border: 1px solid #c3e6cb; }}
        .error {{ background-color: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }}
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
        .no-pets {{ text-align: center; margin: 40px 0; font-size: 18px; color: #666; }}
    </style>
</head>
<body>
    <h1>Pet Gallery</h1>""")

    if message:
        print(f'    <div class="message {message_class}">{html.escape(message)}</div>')

    print('    <div class="gallery">')

    if not pets:
        print('        <div class="no-pets">No pets uploaded yet. Be the first!</div>')
    else:
        for pet in reversed(pets):
            safe_name = html.escape(pet['name'])
            image_url = IMAGE_WEB_PATH + pet['image']
            image_id = pet['image']
            delete_url = f"{script_url}?action=delete&id={image_id}"
            print(f"""        <div class="card">
            <img src="{image_url}" alt="{safe_name}">
            <h3>{safe_name}</h3>
            <a href="{delete_url}" class="delete-link">Delete</a>
        </div>""")

    print(f"""    </div>
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
</html>""")

if __name__ == "__main__":
    try:
        display_gallery()
    except Exception as e:
        print("Content-Type: text/html")
        print()
        print(f"""<!DOCTYPE html>
<html>
<head><title>Error</title></head>
<body>
<h1>Error</h1>
<p>An error occurred: {html.escape(str(e))}</p>
</body>
</html>""")