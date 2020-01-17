from PIL import Image
from pywinusb import hid

def readdata(data):
    global rd
    rd = data
    dt = bytes(data)
    n = dt[1:].find(b'\n')
    s = dt[1:n+1]
    #print(s)

def senddata(rp, data):
    bf=[0]*65
    s = list(data.encode())
    bf[1:len(s)+1] = s
    rp.send(bf)

dev = hid.HidDeviceFilter(vendor_id=0x0416, product_id=0x5020)
try:
    hdev = dev.get_devices()[0]
    hdev.open()
    hdev.set_raw_data_handler(readdata)
    rp0 = hdev.find_output_reports()[0]
    print('OK')
except:
    hdev = None
    print('Nuvoton HID not found !')


def img_head(x=50, y=50, w=100, h=100):
    ar = []
    H, L = divmod(x, 256)
    ar += [L, H]
    H, L = divmod(y, 256)
    ar += [L, H]
    H, L = divmod(w, 256)
    ar += [L, H]
    H, L = divmod(h, 256)
    ar += [L, H]
    return ar

def img_usb_head(ar):
    a1 = [0]+ar+[0]*64
    return a1[0:65]

def run(x=50, y=50, w=100, h=100):
    img_h1 = img_head(x, y, w, h)
    tx = img_usb_head(img_h1)
    rp0.send(tx)
    

def conv_RGB565(px):
    ar=[]
    for r,g,b in px:
        r = (r & 0xF8)<<8
        g = (g & 0xFC)<<(8-5)
        b = b >> 3
        dd = r+g+b
        H, L = divmod(dd, 256)
        ar += [L, H]
    return ar

def img_send(im565):
    ln = len(im565)
    n=0
    while n<ln:
        dt = im565[n:n+64]
        n += 64
        dt = [0] + dt + [0]*64
        dt = dt[0:65]
        rp0.send(dt)
        print('.', end='')
    
im = Image.open("img1.jpg")
ww, hh = im.size
im1 = im.convert('RGB')
px = list(im1.getdata())
im565 = conv_RGB565(px)

run(0, 0, ww, hh)
img_send(im565)
