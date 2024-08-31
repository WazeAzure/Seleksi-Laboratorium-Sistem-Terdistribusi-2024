import pyotp
import hashlib
import base64
import requests
from requests.auth import HTTPBasicAuth  


class API:
    def __init__(self):
        # CONSTANTS
        self.full_name = "Edbert Eddyson Gunawan"
        self.doc_url = "https://drive.google.com/file/d/1imYfu_E6koU_KdXpaV7xoE9d70ebNA80/view?usp=sharing" # <NIM>_<5 karakter pertama sha2-256 sum berkas>.pdf
        self.message = 'hash generated using "sha256sum [file]" command. fc7eab919122d51232d53f48705db42c265b6a6f3a02d6b73e24fac2820ad101. honorable mentions: Wisyendra, Lydia, Flora, Qais (that gave me inspiration. g ada yg gw copast + ke map socially EHE :3)'
        self.nim = "13522039"

        self.totp = self.TOTP()

        self.API = "http://sister21.tech:7787/recruitment/submit/a"

        # PAYLOADS
        self.payloads = {
            'fullname'  : self.full_name,
            'link'      : self.doc_url,
            'message'   : self.message
        }

        # HEADERS
        self.headers = {
            'Content-Type': 'application/json',
            'Authorization': f"Basic {base64.b64encode((self.nim + ':' + self.totp).encode('ascii')).decode('ascii')}",
        }

        self.send()
    
    def TOTP(self):
        idol = "ChungMung"
        shared_secret = "seleksister24" + self.nim + idol
        
        # Encode the shared secret to base32 using UTF-8 encoding
        shared_secret_utf8 = shared_secret.encode('utf-8')
        encoded_secret = base64.b32encode(shared_secret_utf8).decode('utf-8')

        # Generate TOTP compliant with RFC 6238
        totp = pyotp.TOTP(encoded_secret, interval=30, digest='SHA256', digits=8)
        print(totp.now())
        return totp.now()


    def send(self):
        print(self.headers)
        response = requests.post(self.API, json=self.payloads, headers=self.headers)
        print('Status Code:', response.status_code)
        print('Response JSON:', response.text)


if __name__ == '__main__':
    api = API()