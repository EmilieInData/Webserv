#!/usr/bin/python
import cgi
import json
import os
from datetime import datetime
import sys
from urllib.parse import parse_qs

JSON_FILE = './zodiac_list.json'

def get_zodiac_sign(day, month):
    if month == 1: return 'Capricorn' if day < 20 else 'Aquarius'
    elif month == 2: return 'Aquarius' if day < 19 else 'Pisces'
    elif month == 3: return 'Pisces' if day < 21 else 'Aries'
    elif month == 4: return 'Aries' if day < 20 else 'Taurus'
    elif month == 5: return 'Taurus' if day < 21 else 'Gemini'
    elif month == 6: return 'Gemini' if day < 21 else 'Cancer'
    elif month == 7: return 'Cancer' if day < 23 else 'Leo'
    elif month == 8: return 'Leo' if day < 23 else 'Virgo'
    elif month == 9: return 'Virgo' if day < 23 else 'Libra'
    elif month == 10: return 'Libra' if day < 23 else 'Scorpio'
    elif month == 11: return 'Scorpio' if day < 22 else 'Sagittarius'
    elif month == 12: return 'Sagittarius' if day < 22 else 'Capricorn'

try:
    with open(JSON_FILE, 'r') as f:
        zodiac_list = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
    zodiac_list = []

if os.environ.get('REQUEST_METHOD', '') == 'POST':
    input_data = sys.stdin.read(int(os.environ.get('CONTENT_LENGTH', 0)))
else:
    input_data = os.environ.get('QUERY_STRING', '')

form = parse_qs(input_data)
confirmation_message = ""

if form.get('action', [''])[0] == 'delete':
    try:
        index_to_delete = int(form.get('index', [''])[0])
        if 0 <= index_to_delete < len(zodiac_list):
            deleted_person = zodiac_list.pop(index_to_delete)
            confirmation_message = f"<h2>Successfully deleted {deleted_person['first_name']} {deleted_person['last_name']}.</h2>"
            with open(JSON_FILE, 'w') as f:
                json.dump(zodiac_list, f, indent=4)
    except (TypeError, ValueError):
        confirmation_message = "<h2>Error: Invalid index for deletion.</h2>"
elif form.get('dob', [''])[0]:
    first_name = form.get('first_name', [''])[0]
    last_name = form.get('last_name', [''])[0]
    dob_str = form.get('dob', [''])[0]
    if first_name and last_name and dob_str:
        try:
            dob_date = datetime.strptime(dob_str, '%d/%m/%Y')
            day, month = dob_date.day, dob_date.month
            sign = get_zodiac_sign(day, month)
            
            new_entry = {'first_name': first_name, 'last_name': last_name, 'sign': sign}
            zodiac_list.append(new_entry)
            
            with open(JSON_FILE, 'w') as f:
                json.dump(zodiac_list, f, indent=4)
                
            confirmation_message = f"<h2>Successfully added {first_name} {last_name} ({sign})!</h2>"
        except ValueError:
            confirmation_message = "<h2>Error: Invalid date format. Please use DD/MM/YYYY.</h2>"

print("Content-Type: text/html\r\n\r\n")
print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Zodiac List | Le Webserv Fantastique</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <header>
        <nav>
            <a href="/index.html" class="nav-brand">Le Webserv</a>
            <ul>
                <li><a href="/index.html">Home</a></li>
                <li><a href="/about.html">About</a></li>
                <li><a href="/contact.html">Contact</a></li>
                <li class="dropdown">
                    <a href="javascript:void(0)" class="dropbtn">CGI Tests</a>
                    <div class="dropdown-content">
                        <a href="/upload.html">Upload</a>
                        <a href="/cgi-bin/simple_post.py">POST</a>
                        <a href="/cgi-bin/pet_gallery.py">Pet Gallery</a>
                        <a href="/cgi-bin/loop.py">Loop</a>
                        <a href="/cgi-bin/error.py">Error</a>
                        <a href="/cookie_test.html">Cookie Test</a>
                    </div>
                </li>
            </ul>
        </nav>
    </header>
    <main>
        <div class="card">
            <h1>Zodiac List</h1>
""")

print(confirmation_message)

if zodiac_list:
    print('<div class="team-member" style="background: white; padding: 20px; margin-top: 20px;"><table style="width:100%; text-align: left;">')
    print("<tr><th>First Name</th><th>Last Name</th><th>Sign</th><th>Action</th></tr>")
    for index, person in enumerate(zodiac_list):
        delete_link = f'<a href="/cgi-bin/zodiac.py?action=delete&index={index}" style="color: red;">Delete</a>'
        print(f"<tr><td>{person['first_name']}</td><td>{person['last_name']}</td><td>{person['sign']}</td><td>{delete_link}</td></tr>")
    print("</table></div>")
else:
    print("<p>The list is currently empty.</p>")

print("""
            <a href="/zodiac.html" class="btn" style="margin-top: 20px; display: inline-block;">Add Another Person</a>
        </div>
    </main>
    <footer>
        <p>&copy; 2025 42 BCN / WEBSERV</p>
    </footer>
</body>
</html>
""")