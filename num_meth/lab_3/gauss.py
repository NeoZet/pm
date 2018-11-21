def gauss(A, b):
    def _prepare_main_elem():
        for i in range(len(b)):
            for j in range(len(b)):
                if(i != j and A[i][i] == 0 and A[j][i] != 0 and A[i][j] != 0):                    
                    for k in range(len(b)):
                        A[j][k], A[i][k] = A[i][k], A[j][k]
                    b[j],b[i] = b[i],b[j]
                    break

    _prepare_main_elem()
    for i in range(len(b)):
        if A[i][i] == 0:
            return 'e'
        for j in range(i+1, len(b)):
            tmp = A[j][i] / A[i][i]
            for t in range(i, len(b)):
                A[j][t] -= tmp * A[i][t]
            b[j] -= tmp * b[i]
    solve = [0 for i in range(len(b))]
    for i in reversed(range(0, len(b))):
        sum_known_elems = 0
        for j in range(i, len(b)):
            sum_known_elems += A[i][j] * solve[j]
        solve[i] = (b[i] - sum_known_elems) / A[i][i]
    return solve;

M = [[1.80, 2.50, 4.60],
     [3.10, 2.30, -1.20],
     [4.51,-1.80,3.60]]
c = [2.20,3.60,-1.70]

solve = gauss(M,c)
print("Can't solved!") if solve is 'e' else print(solve)


