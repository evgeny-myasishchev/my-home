try:
    from os import urandom
except ImportError:
    import urandom as rnd
    import utime
    rnd.seed(utime.ticks_ms())
    def urandom(n):
        result = []
        for _ in range(n):
            result.append(rnd.getrandbits(8))
        return result