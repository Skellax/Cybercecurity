#!/usr/bin/env python3

import requests 
import os
from bs4 import BeautifulSoup

def download_Img(url, directory):
    response = requests.get(url)
    content = response.content

    file_name = os.path.join(directory, url.split("/")[-1])

    with open(file_name, "wb") as file:
        file.write(content)
        print("Donwload img :", file_name)

url_page = "https://www.mb-riclin.com/"

os.mkdir('data')

repertory = "data"

response = requests.get(url_page)
if (response.status_code == 200):
    content_page = response.content
else:
    print("Error couln't open this url")
    exit()

soup = BeautifulSoup(content_page, 'html.parser')

balise_imgs = soup.find_all('img')

for balise_img in balise_imgs:
    url_img = balise_img["src"]
    download_Img(url_img, repertory)


