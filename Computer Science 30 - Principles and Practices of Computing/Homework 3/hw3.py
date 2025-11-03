
# your functions go here!

def remove(n,l):
    if l == []:
        return []
    else:
        head = l[0]
        tail = l[1:]
        recursiveResult = remove(n, tail)
        if head == n:
            return recursiveResult
        else:
            return [head] + recursiveResult

def countDistinct(l):
    if l == []:
        return 0
    else:
        head = l[0]
        tail = l[1:]
        head_removed = remove(l[0], l)
        recursiveResult = countDistinct(head_removed)
        return 1 + recursiveResult

def insert(n, l):
    if l == []:
        return [n]
    else:
        head = l[0]
        tail = l[1:]
        recursiveResult = insert(n,tail)
        if n <= head:
            return [n] + l
        else:
            return [head] + recursiveResult

def insertionSort(l):
    if l == []:
        return []
    else:
        head = l[0]
        tail = l[1:]
        recursiveResult = insertionSort(tail)
        return insert(head, recursiveResult)


def merge(l1,l2):
    if l1 == []:
        return l2
    elif l2 == []:
        return l1
    elif l1 and l2 == []:
        return []
    else:
        head1 = l1[0]
        head2 = l2[0]
        tail1 = l1[1:] 
        tail2 = l2[1:]
        if head1 > head2:
            recursiveResult = merge(tail2, l1)
            return [head2] + recursiveResult
        else:
            recursiveResult = merge(tail1, l2)
            return [head1] + recursiveResult

    
def mergeSort(l):
    if l == []:
        return []
    elif len(l) == 1:
        return l
    else:
        frontHalf = l[:len(l)//2]
        backHalf = l[len(l)//2:]
        sortedFront = mergeSort(frontHalf)
        sortedBack = mergeSort(backHalf)
        return merge(sortedFront,sortedBack)
        

        
        
