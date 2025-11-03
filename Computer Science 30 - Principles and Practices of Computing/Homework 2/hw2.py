# used in the last part of the homework assignment
import turtle

def fizzBuzz(n):
    if n % 3 == 0:
        return "fizz"
    if n % 5 == 0:
        return "buzz"
    if n % 3 == 0 and n % 5 == 0:
        return "fizz buzz"
    else:
        return str(n)


def age(birth_day, birth_month, birth_year):
    # I used the following resources to help solve this problem:
    # https://www.w3schools.com/python/ref_func_round.asp
    # python-tutor.com
    day_compare = 17
    month_compare = 10
    year_compare = 2024
    
    birth_day_calc = day_compare - birth_day
    month_calc = month_compare - birth_month
    year_calc = year_compare - birth_year

    age_calc = round(year_calc + (month_calc)/12 + (birth_day_calc)/365)

    if birth_day_calc < 0 and birth_month == 10:
        return year_calc - 1
    else:
        return age_calc

def countPos(l):
    # I used the following resource to help solve this problem:
    # python-tutor.com
    if l == []:
        return 0
    else:
        head = l[0]
        tail = l[1:]
        recursiveResult = countPos(tail)
        if head > 0:
            return 1 + recursiveResult
        if head < 0 or head == 0:
            return recursiveResult

def threshold(l,n):
    if l == []:
        return []
    else:
        head = l[0]
        tail = l[1:]
        recursiveResult = threshold(tail,n)
        if head >= n:
            return [head] + recursiveResult
        if head < n:
            return [n] + recursiveResult

# type of input - two integers
# type of output - ordered list of integers
# type of head - an integer
# type of tail - list of integers
# type of recursiveResult - list of integers

def intRange(low,high):
    # I used the following resource to help solve this problem:
    # python-tutor.com
    if high <= low:
        return []
    else:
        head = low
        tail = [low + 1, high - 1]
        tailResult = intRange(tail[0], high)
        return [head] + tailResult

# type of input - list of integers
# type of output - list of lists
# type of head - integer
# type of tail - list of integers
# type of recursiveResult - list of lists
# type of output of intRange - list of integers
def innerLists(l):
    # I used the following resource to help solve this problem:
    # python-tutor.com
    if l == []:
        return []
    else:
        head = l[0]
        tail = l[1:] 
        recursiveResult = innerLists(tail)
        return [intRange(1, l[0]+1)] + recursiveResult


def regularNGon(n, sideLength):
    if n == 0:
        return
    else:
        helper_regularNGon(n, sideLength, 360/n)
        return
    
def helper_regularNGon(sidesLeft, sideLength, angle): 
    if sidesLeft == 0:
        return
    else:
        turtle.forward(sideLength)
        turtle.left(angle)
        helper_regularNGon(sidesLeft - 1, sideLength, angle)
        return


def archSpiral(length, increment, angle, n):
    if n == 0:
        return
    else:
        turtle.forward(length)
        turtle.left(angle)
        length = length + increment
        archSpiral(length, increment, angle, n-1)
        return

    
def logSpiral(length, percentIncrease, angle, n):
    if n == 0:
        return
    else:
        turtle.forward(length)
        turtle.left(angle)
        length = length * (1 + percentIncrease/100)
        logSpiral(length, percentIncrease, angle, n-1)
        return
    
       
    
        
            
    

    
    
