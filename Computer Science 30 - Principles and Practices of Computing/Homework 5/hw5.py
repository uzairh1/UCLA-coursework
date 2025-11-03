import struct
import functools

# My dad helped me with implementing decompress. He is the best :)
# Auguste helped me get started with his Piazza reply at 12:07 a.m. I hope he slept soon afterwards.
# Ana helped A LOT with decompress too...office hours 18-Nov.

'''
You need to implement the six empty functions defined below.
Each function takes a single argument:
  an image represented as a list of lists of pixels, where
  each pixel is a dictionary as described in the HTML file.
Each function returns a new list in exactly that same form.

The inputPPM and outputPPM functions defined at the end of the file should NOT be
called by your code.  Rather, they are provided to help you test your code:
inputPPM allows you to convert PPM image files into lists of lists of pixels that
can be passed as arguments to your functions, and outputPPM allows you to write the
results of your functions to image files so that the images can be viewed.  See
the HTML file for an example of how to use these functions.

But viewing images is not sufficient!  You should also test as we usually do, by
writing tests to ensure that the output of your function is as expected.  See
hw5test.py.
'''

def negate(pixels):
    if pixels == []:
        return []
    else:
        func = lambda lst: list(map(lambda x: {'r': 255 - x['r'], 'g': 255 - x['g'], 'b': 255 - x['b']}, lst))
        return list(map(func, pixels))


                       
def greyscale(pixels):
    if pixels == []:
        return []
    else:
        func = lambda lst : list(map(lambda x: {'r': round((x['r'] + x['g'] + x['b'])/3), 'g': round((x['r'] + x['g'] + x['b'])/3), 'b': round((x['r'] + x['g'] + x['b'])/3)}, lst))
        return list(map(func, pixels))


def upsideDown(pixels):
    if pixels == []:
        return []
    else:
        return pixels[::-1]

def mirrorImage_helper(l): 
    return l[::-1]

def mirrorImage(pixels): 
    if pixels == []:
        return []
    else:
        return list(map(mirrorImage_helper, pixels))

def compress(pixels):
    if pixels == []:
        return []
    else:
        rows_removed = pixels[::2]
        pixels_removed = functools.reduce(lambda partialResult, elem: partialResult + [elem[::2]], rows_removed, [])
        return pixels_removed

def decompress_row(row):
    '''
    1. Double one row of PIXELS: [p1, p2, p3] ---> [p1, p1, p2, p2, p3, p3]
        partialResult and final result of this step (oneRow_PixelsDoubled) = list of pixels
        elem = pixel
    2. Double a ROW that already has its pixels doubled: [p1, p1, p2, p2, p3] ---> [p1, p1, p2, p2, p3, p3], [p1, p1, p2, p2, p3, p3]

    WANT list of list of pixels at the VERY end... which is why there are brackets around the group of oneRow_PixelsDoubled
    '''
    oneRow_PixelsDoubled = functools.reduce(lambda partialResult, pixel: partialResult + [pixel] + [pixel], row, [])
    doubledPixelsRow_Doubled = [oneRow_PixelsDoubled, oneRow_PixelsDoubled]
    return doubledPixelsRow_Doubled

def decompress(pixels):
    '''
    To add multiple groups of decompressed rows together: [[{p1, p1, p2, p2, p3, p3}, {p1, p1, p2, p2, p3, p3}] + [[{p4, p4, p5, p5, p6, p6}, {p4, p4, p5, p5, p6, p6}]]
    partialResult and final result of this FUNCTION: a list of list of pixels
    decompressRow: a list of list of pixels
    '''
    return functools.reduce(lambda partialResult, row: partialResult + decompress_row(row), pixels, [])

# input the PPM image file named fname and convert it to a list of lists of pixels.
# each pixel is an RGB triple, represented using a dictionary with keys "r", "g",
# and "b". each list of pixels represents one row in the image, ordered from top to
# bottom.
def inputPPM(fname):
    f = open(fname, "rb")
    p6Ignore = f.readline()
    dimensions = f.readline().split()
    width = int(dimensions[0])
    height = int(dimensions[1])
    maxIgnore = f.readline()

    pixels = []
    rgbData = [x for x in f.read()]
    f.close()
    for r in range(height):
        row = []
        for c in range(width):
            i = 3 * (r * width + c)
            row.append({"r":rgbData[i], "g":rgbData[i+1], "b":rgbData[i+2]})
        pixels.append(row)
    return pixels


# pixels should be a list of list of RGB triples, in the same format as returned
# by the inputPPM function above.
# this function outputs those pixels to the file named fname as a PPM image.
def outputPPM(pixels, fname):
    f = open(fname, "wb")
    f.write("P6\n".encode())
    width = len(pixels[0])
    height = len(pixels)
    f.write((str(width) + " " + str(height) + "\n").encode())
    f.write((str(255) + "\n").encode())
    bPixels = [[struct.pack('BBB', p["r"], p["g"], p["b"]) for p in r] for r in pixels]
    flatPixels = functools.reduce(lambda x,y: x+y, bPixels)
    f.writelines(flatPixels)
    f.close()


