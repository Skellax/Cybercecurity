#!/usr/bin/env python3

from PIL import Image
from PIL.ExifTags import TAGS
import argparse


class Colors:
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    RED = '\033[91m'
    END = '\033[0m'




def rec_exif(img_path):
    image = Image.open(img_path)
    exifdata = image.getexif()

    for tag_id in exifdata:
        tag = TAGS.get(tag_id, tag_id)
        data = exifdata.get(tag_id)

        if isinstance(data, bytes):
             data = data.decode()
        print(f"{tag}: {data}")
        


parser = argparse.ArgumentParser()
parser.add_argument('path', nargs='+', help='the path of the image only jpg')
args = parser.parse_args()

for arg in args.path:
    if arg.endswith(('.jpg', '.jpeg')):
        print("-------------")
        print(f"{Colors.BLUE}EXIF Metadata for {arg}{Colors.END}")
        rec_exif(arg)
        print()
    else:
        print(f"{Colors.RED}ERROR: wrong format{Colors.END}")


