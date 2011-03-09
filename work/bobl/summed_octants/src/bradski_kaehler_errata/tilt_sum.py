N = 10 # largest index + 1

def tilt_sum(X, Y, book=0):
    """
    prints out the indices contributing to tilt_sum(X, Y)
    """
    # The book has the double sum over y <= Y ...
    for y in range(0, Y+1):
        # ... and | x - X | <= y, which is
        #       -y <= x - X <= y, which is
        #   -y + X <= x <= y + X
        # but this gives a peculiar result.  Instead, it should be
        # ... and | x - X | <= Y - y, which, since y <= Y, is
        #       y - Y <= x - X <= Y - y, which is
        #   y - Y + X <= x <= Y - y + X
        if book: # see the book result
            rhs = y
        else:
            rhs = Y-y
        print (X-rhs) * '      ',
        for x in range(-rhs+X, rhs+X+1):
            if 0 <= x < N and 0 <= y < N:
                print "(%d,%d)" % (x, y),
        print

X = 5
Y = 3
print 'the indices (x,y) used to compute tilt_sum(%d,%d)...' % (X, Y)
print
print '  from the book:'
print
tilt_sum(X, Y, 1)
print
print '  what I think it should be:'
print
tilt_sum(X, Y)
