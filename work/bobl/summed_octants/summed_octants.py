from numpy import ones, zeros, array, arange

def octantWedgeSums(a):
    """
    returns the octant wedge sums of an array `a`.  If `a` is `N` x
    `M`, the result is 8 x `N` x `M`.
    """
    (N,M) = a.shape
    ows = zeros((8, N, M), a.dtype)
    for j in range(M):
        jc = M - 1 - j # "j complement"
        for i in range(N):
            ic = N - 1 - i # "i complement"

            s = a[i,jc]
            if jc < N-1:
                s += ows[0,i,jc+1]
                if i > 0:
                    s += ows[0,i-1,jc+1]
                    if jc < N-2:
                        s -= ows[0,i-1,jc+2]
            ows[0,i,jc] = s

            s = a[i,jc]
            if i > 0:
                s += ows[1,i-1,jc]
                if jc < N - 1:
                    s += ows[1,i-1,jc+1]
                    if i > 1:
                        s -= ows[1,i-2,jc+1]
            ows[1,i,jc] = s

            s = a[i,j]
            if i > 0:
                s += ows[2,i-1,j]
                if j > 0:
                    s += ows[2,i-1,j-1]
                    if i > 1:
                        s -= ows[2,i-2,j-1]
            ows[2,i,j] = s

            s = a[i,j]
            if j > 0:
                s += ows[3,i,j-1]
                if i > 0:
                    s += ows[3,i-1,j-1]
                    if j > 1:
                        s -= ows[3,i-1,j-2]
            ows[3,i,j] = s

            s = a[ic,j]
            if j > 0:
                s += ows[4,ic,j-1]
                if ic < N - 1:
                    s += ows[4,ic+1,j-1]
                    if j > 1:
                        s -= ows[4,ic+1,j-2]
            ows[4,ic,j] = s

            s = a[ic,j]
            if ic < N - 1:
                s += ows[5,ic+1,j]
                if j > 0:
                    s += ows[5,ic+1,j-1]
                    if ic < N - 2:
                        s -= ows[5,ic+2,j-1]
            ows[5,ic,j] = s

            s = a[ic,jc]
            if ic < N - 1:
                s += ows[6,ic+1,jc]
                if jc < N - 1:
                    s += ows[6,ic+1,jc+1]
                    if ic < N - 2:
                        s -= ows[6,ic+2,jc+1]
            ows[6,ic,jc] = s

            s = a[ic,jc]
            if jc < N-1:
                s += ows[7,ic,jc+1]
                if ic < N - 1:
                    s += ows[7,ic+1,jc+1]
                    if jc < N-2:
                        s -= ows[7,ic+1,jc+2]
            ows[7,ic,jc] = s

    return ows

def applyOctangonalKernel(ows, H, W, d, i, j):
    """
    applies an `H` x `W` (both odd) octagonal kernel to element (`i`,
    `j`) of an array whose octagonal wedge table is `ows`.  `d` (which
    is between 0 and min(`H`//2, `W`//2)) is the inset of the octagon.
    """
    assert H % 2 == 1 and H > 0
    assert W % 2 == 1 and W > 0
    hW = (W-1)//2 # half-width
    w = hW - d
    hH = (H-1)//2 # half-height
    assert d <= min(hH, hW)
    h = hH - d
    # This function only deals with interior points, as boundary
    # conditions may be application-specific.
    if i < hH or ows.shape[0] - hH <= i \
            or j < hW or ows.shape[1] - hW <= j:
        raise IndexError, (i, j)
    oijs = (range(8),
           (i+hH , i+h-1,  i-h-1, i-hH-1, i-hH , i-h+1,  i+h+1, i+hH+1),
           (j+w+1, j+hW+1, j+hW,  j+w-1,  j-w-1, j-hW-1, j-hW,  j-w+1))
    sumAll = ows[6,1,0] + ows[7,0,0] # sum of all elements
    return sumAll - sum(ows[oijs])


def octagonalMask(N, M, H, W, d, i, j):
    """
    returns an octagonal bit mask of shape `H` x `W` with an inset of
    `d` centered on `(i, j)` on an array of size `N` x `M`.
    """
    mask = zeros((N, M), int)
    mask[i-H//2:i+H//2+1, j-W//2:j+W//2+1] = 1
    for k in range(d):
        for l in range(d):
            mask[i-H//2+k,j-W//2+l] = 0
            mask[i+H//2-k,j-W//2+l] = 0
            mask[i+H//2-k,j+W//2-l] = 0
            mask[i-H//2+k,j+W//2-l] = 0
    return mask


if __name__ == '__main__':
    N = 9
    M = 9
    print 'original array:'
    if 1:
        a = arange(N*M).reshape((N,M))
    else:
        a = ones((N,M))
    print a
    ows = octantWedgeSums(a)
    W = 5
    H = 5
    d = 1
    # TODO: a function to choose the "most circular" value of d for a
    # given W.
    i = N//2
    j = M//2
    sFast = applyOctangonalKernel(ows, H, W, d, i, j)

    print 'octagonally-masked array:'
    b = octagonalMask(N, M, H, W, d, i, j) * a
    print b
    sSlow = sum(b.flat)

    print '     octal wedge sum:', sFast
    print 'actual octagonal sum:', sSlow
    if sFast == sSlow:
        print 'OK -- results match'
    else:
        print 'ERROR -- results do not match'
        