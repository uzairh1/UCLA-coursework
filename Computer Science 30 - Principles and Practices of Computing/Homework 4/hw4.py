
import functools

# Write your functions here!

def rightTriangles_helper(l):
    side1 = l[0]
    side2 = l[1]
    hyp = l[2]
    if pow(side1, 2) + pow(side2, 2) == pow(hyp, 2):
        return True
    else:
        return False

def rightTriangles(l):
    return list(filter(rightTriangles_helper, l))

# I used Python Tutor to help with this problem.
def threshold(l,n):
    return functools.reduce(lambda partialResult, elem: partialResult + [n] if elem <= n else partialResult + [elem], l, [])                                                                                                                             
                                                                                                                             
def countOccurrences(f,l):
    return len(list(filter(f,l)))

def toUpper(c):
    cEncoded = ord(c)
    if cEncoded >= 65 and cEncoded <= 90:
        return c
    else:
        cEncodedUpper = cEncoded - 32
        return chr(cEncodedUpper)
        
    # pattern: uppercase E = 69, lowercase e = 101; difference = 32

def toLower(c):
    cEncoded = ord(c)
    if cEncoded >= 97 and cEncoded <= 122:
        return c
    else:
        cEncodedLower = cEncoded + 32
        return chr(cEncodedLower)
    
    
def allLower(s):
    listResult = list(map(toLower, s))
    stringResult = functools.reduce(lambda partialResult, elem: partialResult + str(elem), listResult, '')
    return stringResult
    
def capitalize(s):
    return toUpper(s[0]) + allLower(s[1:])

def title_rest(l):
    stringResult = functools.reduce(lambda partialResult, elem: partialResult + str(elem), l, '')
    if len(stringResult) >= 4:
        return capitalize(stringResult)
    else:
        return allLower(stringResult)

def title(l):
    firstWord = capitalize(l[0])
    list_rest = list(map(title_rest, l[1:]))
    return [firstWord] + list_rest
