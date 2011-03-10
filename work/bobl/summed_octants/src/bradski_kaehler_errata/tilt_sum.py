N = 5 # largest index + 1

BOOK = 0
DOC  = 1
ME   = 2

def tilt_sum(X, Y, source):
    """
    prints out the indices contributing to tilt_sum(X, Y)
    """
    if source in (BOOK, ME):
        # The book has the double sum over y <= Y ...
        for y in range(0, Y+1):
            # ... and | x - X | <= y, which is
            #       -y <= x - X <= y, which is
            #   -y + X <= x <= y + X
            # but this gives a peculiar result.
            #
            # I think it should be:
            # ... and | x - X | <= Y - y, which, since y <= Y, is
            #       y - Y <= x - X <= Y - y, which is
            #   y - Y + X <= x <= Y - y + X
            if source == BOOK: # see the book result
                rhs = y
            else:
                assert(source == ME)
                rhs = Y-y
            print (X-rhs) * '      ',
            for x in range(-rhs+X, rhs+X+1):
                if 0 <= x < N and 0 <= y < N:
                    print "(%d,%d)" % (x, y),
            print
    else:
        assert(source == DOC)
        # The book has the double sum over y < Y ...
        for y in range(0, Y):
            #    | x - X + 1 | <= Y - y - 1
            #     -(Y - y - 1) <= x - X + 1 <= Y - y - 1
            #       -Y + y + 1 <= x - X + 1 <= Y - y - 1
            #    X - Y + y + 1 <= x + 1     <= X + Y - y - 1
            #    X - Y + y     <= x         <= X + Y - y - 2
            xlo = X - Y + y
            xhi = X + Y - y - 2
            print xlo * '      ',
            for x in range(xlo, xhi+1):
                print "(%d,%d)" % (x, y),
            print

X = 2
Y = 2
print 'the indices (x,y) used to compute tilt_sum(%d,%d)...' % (X, Y)
print
print '  from the book:'
print
tilt_sum(X, Y, BOOK)
print
print '  what the documentation says:'
print
tilt_sum(X, Y, DOC)
print
print '  what I think it should be:'
print
tilt_sum(X, Y, ME)
