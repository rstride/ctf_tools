import requests

url = "http://challenge01.root-me.org/web-serveur/ch63/"

response = requests.get(url + "token")
token = response.json().get("token")
print("Token:", token)

headers = {
    "Authorization": f"Bearer {token}"
}

response = requests.post(url + "admin", headers=headers)

if "Token is revoked" in response.text:
    print("Token is revoked, cannot access admin page.")
else:
    print(response.text)