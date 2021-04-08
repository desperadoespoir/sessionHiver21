def carre(taille):
    pensize(taille)
    fd(taille)
    pensize(1)
    
def triangle(base):
    size = 1
    half = base >> 1
    while size <= base:
        fd(1)
        size += 2
        pensize(size)
    pensize(1)
    
def ligne(base, taille, n):
    for _ in range(n):
        triangle(base)
        carre(taille)
        # breakpoint()
pu()
goto(-100,0)
pd()
ligne(20,10,5) 