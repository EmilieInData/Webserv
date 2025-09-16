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
    
    # Handle delete action
    if params.get("action", [None])[0] == 'delete':
        if handle_delete(params):
            message = "Pet deleted successfully!"
        else:
            message = "Failed to delete pet."
    
    # Handle upload (POST)
    if os.environ.get("REQUEST_METHOD") == "POST":
        success, msg = handle_upload()
        message = msg
    
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
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gallery | Le Webserv Fantastique</title>
    <link rel="stylesheet" href="/static/style.css">
    <style>
        /* Small addition for delete link since it's not in the main style */
        .delete-link {{
            color: #e74c3c;
            text-decoration: none;
            font-size: 0.9em;
            margin-top: 5px;
            display: inline-block;
        }}
        .delete-link:hover {{
            text-decoration: underline;
        }}
        .message {{
            max-width: 600px;
            margin: 20px auto;
            padding: 15px;
            border-radius: 8px;
            text-align: center;
            font-weight: bold;
            background-color: rgba(255, 255, 255, 0.8);
            border: 1px solid var(--primary-color);
        }}
    </style>
</head>
<body>
    <header>
        <nav>
            <a href="index.html" class="nav-brand">Le Webserv</a>
            <ul>
                <li><a href="index.html">Home</a></li>
                <li><a href="about.html">About</a></li>
                <li><a href="contact.html">Contact</a></li>
                <li><a href="{script_url}" class="active">Gallery</a></li>
            </ul>
        </nav>
    </header>

    <main>
        <div class="card">
            <h1>Pet Gallery</h1>""")
    
    if message:
        print(f'            <p class="message">{html.escape(message)}</p>')
    
    print("""            <div class="team-gallery">""")
    
    if not pets:
        print('                <p>No pets uploaded yet. Be the first!</p>')
    else:
        for pet in reversed(pets):
            safe_name = html.escape(pet['name'])
            image_url = IMAGE_WEB_PATH + pet['image']
            image_id = pet['image']
            delete_url = f"{script_url}?action=delete&id={image_id}"
            print(f"""                <div class="team-member">
                    <img src="{image_url}" alt="{safe_name}">
                    <p><strong>{safe_name}</strong></p>
                    <a href="{delete_url}" class="delete-link">Delete</a>
                </div>""")

    print(f"""            </div>

            <h1>Upload a New Pet</h1>
            <form action="{script_url}" method="post" enctype="multipart/form-data" class="contact-form">
                <input type="text" id="pet_name" name="pet_name" placeholder="Pet's Name" required>
                <input type="file" id="pet_picture" name="pet_picture" accept="image/*" required>
                <button type="submit" class="btn">Upload Pet</button>
            </form>
        </div>
    </main>

    <footer>
        <p>&copy; {2025} 42 BCN / WEBSERV</p>
    </footer>
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