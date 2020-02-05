from urandom import getrandbits

def urandom(n):
    result = []
    for i in range(n):
        result.append(getrandbits(8))
        pass
    return result