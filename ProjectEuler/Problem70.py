import math

LIMIT = 10_000_000

def sieve_totients(n):
    phi = list(range(n))
    
    for i in range(2, n):
        if phi[i] == i:
            for j in range(i, n, i):
                phi[j] -= phi[j] // i
    return phi


def is_permutation(a, b):
    return sorted(str(a)) == sorted(str(b))


def solve():
    phi = sieve_totients(LIMIT)

    best_n = 0
    best_ratio = float('inf')

    for n in range(2, LIMIT):
        phi_n = phi[n]

        if is_permutation(n, phi_n):
            ratio = n / phi_n
            if ratio < best_ratio:
                best_ratio = ratio
                best_n = n

    return best_n, best_ratio


print(solve())