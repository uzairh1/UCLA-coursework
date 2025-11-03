
# a few problems will use functions from this library
import math

def totalSeconds(second, minutes, hours):
    # FIX THIS! - good to go, Professor Millstein et. al. :)
    secondsFromSecond = second
    secondsFromMinutes = 60 * minutes
    secondsFromHours = 3600 * hours
    totalSeconds = secondsFromSecond + secondsFromMinutes + secondsFromHours
    return(totalSeconds)


# write the function money
# I used the following resources:
# https://www.w3schools.com/python/ref_func_round.asp and
# https://www.investor.gov/financial-tools-calculators/calculators/compound-interest-calculator and
# https://www.thecalculatorsite.com/finance/calculators/compound-interest-formula
def money(balance, interestRate, years):
    principal = balance # P in P(1 + r/n)^nt
    exponent = years # nt in P(1 + r/n)^nt
    innerFactor = 1 + (interestRate/100) # (1 + r/n) in P(1 + r/n)^nt
    compoundInterest = principal * pow(innerFactor, exponent)
    compoundInterest_rounded = round(compoundInterest, 2)
    return compoundInterest_rounded


# write the function distanceFromNearestSquare
# I used the following resources:
# https://www.w3schools.com/python/ref_func_abs.asp and
# https://www.w3schools.com/python/ref_func_abs.asp (for inspiration) and
# https://www.geeksforgeeks.org/closest-perfect-square-and-its-distance/(for inspiration)
def distanceFromNearestSquare(integer):
    squareRootOfInteger = pow(integer, 0.5)
    rounded_squareRootOfInteger = int(round(squareRootOfInteger, 0))
    NearestSquare = pow(rounded_squareRootOfInteger, 2)
    distanceFromNearestSquare = abs(integer - NearestSquare)
    return distanceFromNearestSquare


# write the function isPrefix
def isPrefix(string1, string2):
    prefix = min(string1, string2)
    not_prefix = max(string1, string2)
    prefix_sliced = prefix[0:len(prefix)]
    not_prefix_sliced = not_prefix[0:len(prefix)]
    return prefix == not_prefix_sliced


# write the function fromTotalSeconds
# I used the following resources:
# https://www.w3computing.com/python/arithmetic-operators/ and
# https://www.w3schools.com/python/ref_func_round.asp and
# my tech-y roommate (Tymofiy Kornyevev)
def fromTotalSeconds(seconds):
    hoursFromSeconds = seconds / 3600
    wholeNumber_hoursFromSeconds = seconds // 3600
    remainderFrom_hoursFromSeconds = hoursFromSeconds - wholeNumber_hoursFromSeconds
    minutesFromSeconds = (remainderFrom_hoursFromSeconds) * 60
    wholeNumber_minutesFromSeconds = round(minutesFromSeconds)
    secondsFromSeconds = (minutesFromSeconds - wholeNumber_minutesFromSeconds) * 60
    wholeNumber_secondsFromSeconds = round(secondsFromSeconds)
    fromTotalSeconds_list = [wholeNumber_secondsFromSeconds, wholeNumber_minutesFromSeconds, wholeNumber_hoursFromSeconds]
    return fromTotalSeconds_list



        
