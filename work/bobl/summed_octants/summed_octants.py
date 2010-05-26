from numpy import ones, zeros

def octantWedgeSums(A):
    N = A.shape[0]
    assert N == A.shape[1]
    O = zeros((8, N, N), A.dtype)
    # fill in the octant tables
    for j in range(N):
        jc = N - 1 - j # "j complement"
        for i in range(N):
            ic = N - 1 - i # "i complement"

            octant = 0
            O[octant,i,jc] = A[i,jc]
            if jc < N-1:
                O[octant,i,jc] += O[octant,i,jc+1]
                if i > 0:
                    O[octant,i,jc] += O[octant,i-1,jc+1]
                    if jc < N-2:
                        O[octant,i,jc] -= O[octant,i-1,jc+2]

            octant = 1
            O[octant,i,jc] = A[i,jc]
            if i > 0:
                O[octant,i,jc] += O[octant,i-1,jc]
                if jc < N - 1:
                    O[octant,i,jc] += O[octant,i-1,jc+1]
                    if i > 1:
                        O[octant,i,jc] -= O[octant,i-2,jc+1]

            octant = 2
            O[octant,i,j] = A[i,j]
            if i > 0:
                O[octant,i,j] += O[octant,i-1,j]
                if j > 0:
                    O[octant,i,j] += O[octant,i-1,j-1]
                    if i > 1:
                        O[octant,i,j] -= O[octant,i-2,j-1]

            octant = 3
            O[octant,i,j] = A[i,j]
            if j > 0:
                O[octant,i,j] += O[octant,i,j-1]
                if i > 0:
                    O[octant,i,j] += O[octant,i-1,j-1]
                    if j > 1:
                        O[octant,i,j] -= O[octant,i-1,j-2]

            octant = 4
            O[octant,ic,j] = A[ic,j]
            if j > 0:
                O[octant,ic,j] += O[octant,ic,j-1]
                if ic < N - 1:
                    O[octant,ic,j] += O[octant,ic+1,j-1]
                    if j > 1:
                        O[octant,ic,j] -= O[octant,ic+1,j-2]

            octant = 5
            O[octant,ic,j] = A[ic,j]
            if ic < N - 1:
                O[octant,ic,j] += O[octant,ic+1,j]
                if j > 0:
                    O[octant,ic,j] += O[octant,ic+1,j-1]
                    if ic < N - 2:
                        O[octant,ic,j] -= O[octant,ic+2,j-1]

            octant = 6
            O[octant,ic,jc] = A[ic,jc]
            if ic < N - 1:
                O[octant,ic,jc] += O[octant,ic+1,jc]
                if jc < N - 1:
                    O[octant,ic,jc] += O[octant,ic+1,jc+1]
                    if ic < N - 2:
                        O[octant,ic,jc] -= O[octant,ic+2,jc+1]

            octant = 7
            O[octant,ic,jc] = A[ic,jc]
            if jc < N-1:
                O[octant,ic,jc] += O[octant,ic,jc+1]
                if ic < N - 1:
                    O[octant,ic,jc] += O[octant,ic+1,jc+1]
                    if jc < N-2:
                        O[octant,ic,jc] -= O[octant,ic+1,jc+2]

    return O

N = 9
A = ones((N,N), int)

print A
print
O = octantWedgeSums(A)
print O

# sum over each octagonal kernel
W = 3
d = 2
w = W - d
i = N // 2
j = N // 2
s0 = O[0,N-1,0] + O[1,N-2,0]
print s0
res = s0 - (
      O[0,i+W,j+w+1] + O[1,i,j+W+1]
    + O[2,i-w-1,j+W] + O[3,i-W-1,j]
    + O[4,i-W,j-w-1] + O[5,i,j-W-1]
    + O[6,i+w+1,j-W] + O[7,i+W+1,j])
print res
