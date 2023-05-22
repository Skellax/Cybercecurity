import random
import binascii
import os
import hmac
import hashlib
import struct
import time
import sys
import string
import base64


class Colors:
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    RED = '\033[91m'
    END = '\033[0m'


def get_hotp_token(key, counter, digits=6):
    key_decrypt = binascii.unhexlify(key)
    counter_bytes = struct.pack(b'>Q', counter)
    h = hmac.new(key=key_decrypt, msg=counter_bytes, digestmod=hashlib.sha1).hexdigest()
    O = int(h[-1], 16)
    DynamicBinaryCode = int(h[O * 2:O * 2 + 8], 16) & 0x7fffffff
    return str(DynamicBinaryCode)[-digits:]


def get_totp_token(secret):
    with open(secret, 'rb') as f:
        hex_key = binascii.hexlify(f.read().strip())
    if len(hex_key) != 64:
        print("error: Key must be 64 hexadecimal characters.")
        return None
    return get_hotp_token(hex_key, int(time.time() // 30))


def store_key(filename, hex_key):
    with open(filename, 'wb') as f:
        f.write(binascii.unhexlify(hex_key))
    print(f"{Colors.GREEN}Key was successfully saved in ft_otp.key{Colors.END}")


def main(args):
    if len(args) == 3:
        actions = args[1]
        filename = args[2]
        if actions == "-g":
            try:
                f = open(filename, 'r')
            except OSError:
                print(f"{Colors.RED}Could not open/read file: {filename} {Colors.END}")
                sys.exit()
            with f:
                hex_key = f.read().strip()
                if len(hex_key) != 64 or not all(c in string.hexdigits for c in hex_key):
                    print(f"{Colors.RED}Key must be 64 hexadecimal characters.{Colors.END}")
                    sys.exit()
                store_key('ft_otp.key', hex_key)

        elif actions == "-k":
            if os.path.exists('ft_otp.key'):
                print(get_totp_token(filename))
            else:
                print(f"{Colors.RED}ft_otp.key doesn't exist!{Colors.END}")
    else:
        print(f"{Colors.RED}Wrong arguments.{Colors.END}")


if __name__ == "__main__":
    main(sys.argv)