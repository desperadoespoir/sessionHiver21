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
    


def spirale(base,taille, nInit):
    for i in range(1, 11):
        ligne(base,taille,(i*nInit) >> 1); lt(90)
        ligne(base,taille,(i*nInit) >> 1); lt(90)

def spiraleTournante(taille1,taille2, nbFormes, vAng):
    angle = 0
    while True:
        clear()
        ht()
        rt(angle)
        spirale(taille1, taille2, nbFormes)
        # breakpoint()
        angle += vAng 
        sleep(0.01)
        
spiraleTournante(15,10,2,2)