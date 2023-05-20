#!/usr/bin/env python3

import requests
import shutil
import os
from bs4 import BeautifulSoup
import argparse



class Colors:
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    RED = '\033[91m'
    END = '\033[0m'


#Fonction permettant de récuperer les images via les balises srcs des images 

def download_img(url, directory):
    response = requests.get(url)
    content = response.content

    file_name = os.path.join(directory, url.split("/")[-1])

    with open(file_name, "wb") as file:
        file.write(content)
    print("Downloaded image:", file_name)


#Fonction permettant de recuperer les differents liens pour la récursive

def change_url(url_base, url, level, arg_level, directory):
    response = requests.get(url)
    if response.status_code == 200:
        content_page = response.content
    else:
        print(f"{Colors.RED}Error: Couldn't open this URL{Colors.END}")
        exit()

    soup = BeautifulSoup(content_page, 'html.parser')

    balise_imgs = soup.find_all('img')

    for balise_img in balise_imgs:
        url_img = balise_img["src"]
        if url_img.find("wordpress") != -1:
            url_img = url_img.replace("wordpress", "www")
        if url_img.endswith(('jpeg', 'jpg', 'gif', 'png', 'bmp')):
            print(f"{Colors.BLUE}---Level: {level}  {Colors.END}")
            download_img(url_img, directory)

    if level < arg_level:
        balise_a = soup.find_all('a')

        for balise_href in balise_a:
            url_href = balise_href.get("href")
            if url_href and url_href.startswith(url_base):
                change_url(url_base, url_href, level + 1, arg_level, directory)


#Parsing des arugments

parser = argparse.ArgumentParser()
parser.add_argument('url', help="the URL of the website")
parser.add_argument('-r', '--recursive', action='store_true')
parser.add_argument('-l', '--level', type=int, default=5, choices=range(1, 5), help='the level of depth')
parser.add_argument('-p', '--path', default='./data', help='the name of the directory')
args = parser.parse_args()
level = 1

#Creation du nom du dossier si il existe on le suprimme avec son contenu puis on le recréer

if os.path.exists(args.path):
    shutil.rmtree(args.path)

if not os.path.exists(args.path):
    os.mkdir(args.path)

if not args.recursive:
    change_url(args.url, args.url, level, 1, args.path)
else:
    change_url(args.url, args.url, level, args.level, args.path)

print(f"{Colors.GREEN}Operation successful!{Colors.END}")
