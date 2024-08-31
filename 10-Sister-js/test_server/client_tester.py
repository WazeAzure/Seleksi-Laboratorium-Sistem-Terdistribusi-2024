import requests

URL = "http://127.0.0.1:1237"

# TEST CASE 1
print("=============== CASE 1 ===============")
param1 = {
    "name": "edbert",
    "gender": "boy"
}
header1 = {
    "Accept": "text/plaintext"
}
res = requests.get(URL+"/template", params=param1, headers=header1)
print(res.text)
print()

# TEST CASE 2
print("=============== CASE 2 ===============")
param1 = {
    "name": "janice",
    "gender": "girl"
}
header1 = {
    "Accept": "text/plaintext"
}
res = requests.get(URL+"/template", params=param1, headers=header1)
print(res.text)
print()

# TEST CASE 3 - ceritanya tembak ke API
print("=============== CASE 3 ===============")
param1 = {
    "name": "janice",
    "gender": "girl"
}
header1 = {
    "Accept": "application/json"
}
res = requests.get(URL+"/leak_dict", params=param1, headers=header1)
print(res.text)
print()

# TEST CASE 4 - POST save to file
print("=============== CASE 4 ===============")
param1 = {
    "file": "edbert-json.txt"
}
header1 = {
    "Content-Type": "application/json",
    "Accept": "text/html"
}
data = {
    "name": "edbert",
}
res = requests.post(URL+"/save-file", params=param1, headers=header1, json=data)
print(res.text)
print()

# TEST CASE 5 - POST save to file
print("=============== CASE 5 ===============")
param1 = {
    "file": "edbert-plaintext.txt"
}
header1 = {
    "Content-Type": "text/plain",
    "Accept": "text/html"
}
data = {
    "name": "aowkoaskodka dari edbert2"
}
res = requests.post(URL+"/save-file", params=param1, headers=header1, data=data)
print(res.text)
print()

# TEST CASE 6 - POST save to file
print("=============== CASE 6 ===============")
param1 = {
    "file": "edbert-urlencoded.txt"
}
header1 = {
    "Content-Type": "application/x-www-form-urlencoded",
    "Accept": "text/html"
}
data = {
    "name": "aowkoaskodka dari edbert3"
}
res = requests.post(URL+"/save-file", params=param1, headers=header1, data=data)
print(res.text)
print()

# TEST CASE 7 - PUT file
print("=============== CASE 7 ===============")
param1 = {
    "file": "edbert-plaintext.txt"
}
header1 = {
    "Content-Type": "application/x-www-form-urlencoded",
    "Accept": "text/html"
}
data = {
    "name": "aowkoaskodka dari edbert5"
}
res = requests.put(URL+"/put", params=param1, headers=header1, data=data)
print(res.text)
print()

input("Press enter to continue...")
# TEST CASE 8 - DELETE file
print("=============== CASE 8 ===============")
param1 = {
    "file": "edbert-json.txt"
}
header1 = {
    "Content-Type": "application/x-www-form-urlencoded",
    "Accept": "text/html"
}
data = {
    "name": "aowkoaskodka dari edbert5"
}
res = requests.delete(URL+"/del", params=param1)
print(res.text)
print()
