
# Read the homework description carefully!
# - I always do :)


def halveEvens(l): 
    result = []
    for x in l:
        if x % 2 == 0:
            result = result + [x // 2]
    return result

            
def splitEveryOther(l):
    result_odd = []
    result_even = []
    i = 0
    for i in range(len(l)):
        if i % 2 != 0:
            result_odd = result_odd + [l[i]]
            i = i + 1
        else:
            result_even = result_even + [l[i]]
            i = i + 1
    return [result_even, result_odd]

def isSorted(l):
    result = True 
    i = 0
    for i in range(len(l)):
        if l[i-1] <= l[i]:
            result = True
        else:
            result = False
    return result

# I used Python Tutor to help me implement dotProduct.

def dotProduct(l1, l2): 
    result = 0
    i = 0
    for i in range(len(l1)):
        result = result + (l1[i] * l2[i])
        i = i + 1
    return result

# I used Python Tutor - and got some help from Ana on 25-Nov - to help me implement negate.
# And my tech-y roommate (Tymofiy Kornyevev) attempted to help me too.

def negate(pixels):
    result = []
    for row in pixels:
        innerResult = []
        for pixel in row:
            newPixel = {'r': 255 - pixel['r'], 'g': 255 - pixel['g'], 'b': 255 - pixel['b']}
            innerResult = innerResult + [newPixel]
        result = result + [innerResult]
    return result

# I used Python Tutor to help me implement toDigitList.

def toDigitList(n):
    rest = []
    oldN = n
    newN = n
    while newN > 9:
        oldN = newN
        newN = newN // 10
        remaining = oldN % 10
        rest = [remaining] + rest
    result = [newN] + rest
    return result

# I used Python Tutor to help me implement digitalRootAndPersistence.

def digitalRootAndPersistence(n):
    nList = toDigitList(n)
    sumNList = sum(nList)
    c = 1
    while sumNList >= 10:
        nList = toDigitList(sumNList)
        sumNList = sum(nList)
        c = c + 1
    result = {"root": sumNList, "persistence": c}
    return result
            
            
