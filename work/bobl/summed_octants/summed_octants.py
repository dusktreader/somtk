from numpy import ones, zeros, array

def octantWedgeSums(a):
    N = a.shape[0]
    assert N == a.shape[1]
    ows = zeros((8, N, N), a.dtype)
    # fill in the octant tables
    for j in range(N):
        jc = N - 1 - j # "j complement"
        for i in range(N):
            ic = N - 1 - i # "i complement"

            octant = 0
            ows[octant,i,jc] = a[i,jc]
            if jc < N-1:
                ows[octant,i,jc] += ows[octant,i,jc+1]
                if i > 0:
                    ows[octant,i,jc] += ows[octant,i-1,jc+1]
                    if jc < N-2:
                        ows[octant,i,jc] -= ows[octant,i-1,jc+2]

            octant = 1
            ows[octant,i,jc] = a[i,jc]
            if i > 0:
                ows[octant,i,jc] += ows[octant,i-1,jc]
                if jc < N - 1:
                    ows[octant,i,jc] += ows[octant,i-1,jc+1]
                    if i > 1:
                        ows[octant,i,jc] -= ows[octant,i-2,jc+1]

            octant = 2
            ows[octant,i,j] = a[i,j]
            if i > 0:
                ows[octant,i,j] += ows[octant,i-1,j]
                if j > 0:
                    ows[octant,i,j] += ows[octant,i-1,j-1]
                    if i > 1:
                        ows[octant,i,j] -= ows[octant,i-2,j-1]

            octant = 3
            ows[octant,i,j] = a[i,j]
            if j > 0:
                ows[octant,i,j] += ows[octant,i,j-1]
                if i > 0:
                    ows[octant,i,j] += ows[octant,i-1,j-1]
                    if j > 1:
                        ows[octant,i,j] -= ows[octant,i-1,j-2]

            octant = 4
            ows[octant,ic,j] = a[ic,j]
            if j > 0:
                ows[octant,ic,j] += ows[octant,ic,j-1]
                if ic < N - 1:
                    ows[octant,ic,j] += ows[octant,ic+1,j-1]
                    if j > 1:
                        ows[octant,ic,j] -= ows[octant,ic+1,j-2]

            octant = 5
            ows[octant,ic,j] = a[ic,j]
            if ic < N - 1:
                ows[octant,ic,j] += ows[octant,ic+1,j]
                if j > 0:
                    ows[octant,ic,j] += ows[octant,ic+1,j-1]
                    if ic < N - 2:
                        ows[octant,ic,j] -= ows[octant,ic+2,j-1]

            octant = 6
            ows[octant,ic,jc] = a[ic,jc]
            if ic < N - 1:
                ows[octant,ic,jc] += ows[octant,ic+1,jc]
                if jc < N - 1:
                    ows[octant,ic,jc] += ows[octant,ic+1,jc+1]
                    if ic < N - 2:
                        ows[octant,ic,jc] -= ows[octant,ic+2,jc+1]

            octant = 7
            ows[octant,ic,jc] = a[ic,jc]
            if jc < N-1:
                ows[octant,ic,jc] += ows[octant,ic,jc+1]
                if ic < N - 1:
                    ows[octant,ic,jc] += ows[octant,ic+1,jc+1]
                    if jc < N-2:
                        ows[octant,ic,jc] -= ows[octant,ic+1,jc+2]

    return ows

def applyOctangonalKernel(ows, W, d, i, j):
    """
    This applies a 2`W`+1 x 2`W`+1 octagonal kernel to element (`i`,
    `j`) of an array whose octagonal wedge table is `ows`.  `d` (which
    is between 0 and `W`) is the inset of the octagon.
    """
    w = W - d
    s0 = ows[0,-1,0] + ows[1,-2,0] # sum of all elements
    if 0:
        inds = array(((0,i+W,j+w+1), (1,i+w-1,j+W+1),
                (2,i-w-1,j+W), (3,i-W-1,j+w-1),
                (4,i-W,j-w-1), (5,i-w+1,j-W-1),
                (6,i+w+1,j-W), (7,i+W+1,j-w+1)))
        print inds
        # TODO: some expression like this:
        result = s0 - sum(ows[inds].flat)
    else:
        result = s0 - (
              ows[0,i+W,j+w+1] + ows[1,i+w-1,j+W+1]
            + ows[2,i-w-1,j+W] + ows[3,i-W-1,j+w-1]
            + ows[4,i-W,j-w-1] + ows[5,i-w+1,j-W-1]
            + ows[6,i+w+1,j-W] + ows[7,i+W+1,j-w+1])
    return result

N = 51
a = ones((N,N), int)
ows = octantWedgeSums(a)
W = 2
d = 0
# TODO: a function to choose the "most circular" value of d for a
# given W.
# TODO: allow varying width and height
# TODO: make "W" a full, rather than half-width
i = N // 2
j = N // 2
print applyOctangonalKernel(ows, W, d, i, j)

