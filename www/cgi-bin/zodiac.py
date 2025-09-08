#!/usr/bin/python3
import cgi
import json
import os
from datetime import datetime

# The JSON file will be stored in the same directory as the script.
JSON_FILE = './zodiac_list.json'

def get_zodiac_sign(day, month):
    if month == 1:
        return 'Capricorn' if day < 20 else 'Aquarius'
    elif month == 2:
        return 'Aquarius' if day < 19 else 'Pisces'
    elif month == 3:
        return 'Pisces' if day < 21 else 'Aries'
    elif month == 4:
        return 'Aries' if day < 20 else 'Taurus'
    elif month == 5:
        return 'Taurus' if day < 21 else 'Gemini'
    elif month == 6:
        return 'Gemini' if day < 21 else 'Cancer'
    elif month == 7:
        return 'Cancer' if day < 23 else 'Leo'
    elif month == 8:
        return 'Leo' if day < 23 else 'Virgo'
    elif month == 9:
        return 'Virgo' if day < 23 else 'Libra'
    elif month == 10:
        return 'Libra' if day < 23 else 'Scorpio'
    elif month == 11:
        return 'Scorpio' if day < 22 else 'Sagittarius'
    elif month == 12:
        return 'Sagittarius' if day < 22 else 'Capricorn'

# Load existing data from the JSON file
try:
    with open(JSON_FILE, 'r') as f:
        zodiac_list = json.load(f)
except (FileNotFoundError, json.JSONDecodeError):
    zodiac_list = []

# Process form data
form = cgi.FieldStorage()
first_name = form.getvalue('first_name')
last_name = form.getvalue('last_name')
dob_str = form.getvalue('dob')
confirmation_message = ""

if first_name and last_name and dob_str:
    try:
        # This line has been changed to parse the new format
        dob_date = datetime.strptime(dob_str, '%d/%m/%Y')
        day, month = dob_date.day, dob_date.month
        sign = get_zodiac_sign(day, month)
        
        new_entry = {
            'first_name': first_name,
            'last_name': last_name,
            'sign': sign
        }
        zodiac_list.append(new_entry)
        
        # Save the updated list back to the file
        with open(JSON_FILE, 'w') as f:
            json.dump(zodiac_list, f, indent=4)
            
        confirmation_message = f"<h2>Successfully added {first_name} {last_name} ({sign})!</h2>"
    except ValueError:
        confirmation_message = "<h2>Error: Invalid date format. Please use DD/MM/YYYY.</h2>"

# Print the HTTP headers and HTML response
print("Content-Type: text/html\r\n\r\n")
print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Zodiac List</title>
    <style>
        body { font-family: sans-serif; background-color: #f4f4f9; color: #333; }
        .container { max-width: 600px; margin: 50px auto; padding: 20px; background: #fff; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        h1, h2 { text-align: center; color: #5a5a5a; }
        table { width: 100%; margin-top: 20px; border-collapse: collapse; }
        th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #007bff; color: white; }
        a { display: block; text-align: center; margin-top: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Zodiac List</h1>
""")

print(confirmation_message)

if zodiac_list:
    print("<table>")
    print("<tr><th>First Name</th><th>Last Name</th><th>Zodiac Sign</th></tr>")
    for person in zodiac_list:
        print(f"<tr><td>{person['first_name']}</td><td>{person['last_name']}</td><td>{person['sign']}</td></tr>")
    print("</table>")
else:
    print("<p>The list is currently empty.</p>")

print("""
        <a href="/zodiac.html">Add Another Person</a>
    </div>
</body>
</html>
""")