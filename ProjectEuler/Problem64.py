import math

def period_length(n):
    a0 = int(math.isqrt(n))

    if a0 * a0 == n:
        return 0

    m = 0
    d = 1
    a = a0
    period = 0

    while True:
        m = d * a - m
        d = (n - m * m) // d
        a = (a0 + m) // d

        period += 1

        if a == 2 * a0:
            break

    return period


def count_odd_periods(limit):
    count = 0

    for n in range(2, limit + 1):
        if period_length(n) % 2 == 1:
            count += 1

    return count


result = count_odd_periods(10000)
print(result)