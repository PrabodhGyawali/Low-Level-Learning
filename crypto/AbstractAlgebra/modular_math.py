
def gcd(a, b):
    if (b == 0):
        return a
    else:
        return gcd(b, a % b)
    

def modular_inverse(a, n):
    """
    Solves for az = 1 (mod n)
    """

    if a > n:
        a = a % n

    d = gcd(a, n)
    if d > 1:
        return None
    else:
        for i in range(n):
            if (a*i) % n == 1:
                return i

def chinese_remainder_theorem(*n):
    # pairwise coprimality check
    for i in range(len(n)):
        for j in range(len(n)):
            if (i!=j):
                d = gcd(n[i], n[j])
                if d != 1:
                    return None

    z = 0
    prod_n = 1

    for i in range(len(n)):
        prod_n *= n[i]

    for i in range(len(n)):
        n_i = prod_n / n[i]
        m = modular_inverse(n_i, n[i])
        z += m * n_i

    return z
                
    
