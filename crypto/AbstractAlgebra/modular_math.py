
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

def chinese_remainder(remainders, moduli):
    # pairwise coprimality check

    k = len(moduli)
    for i in range(k):
        for j in range(k):
            if (i!=j):
                d = gcd(moduli[i], moduli[j])
                if d != 1:
                    return None

    z = 0
    N = 1
    
    for m in moduli:
        N *= m

    for i in range(k):
        ni = moduli[i]
        ai = remainders[i]
        Mi = N / ni
        wi = modular_inverse(Mi, ni)
        z += wi * Mi * ai

    return z % N
                
    
