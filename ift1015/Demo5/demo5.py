# Solution  Exo  3 :

def polygoneReg(cote, nbCotes):
 for _ in range(nbCotes):
    fd(cote)
    rt(360/nbCotes)
    
    
def triangle(cote):
 polygoneReg(cote, 3)


def losange(cote):
 for _ in range(2):
    fd(cote)
    rt(120)
    fd(cote)
    rt(60)
    
def hexagone(cote):
 for _ in range(6):
    triangle(cote)
    rt(60)
    
    
def cube(cote):
 for _ in range(3):
    losange(cote)
    rt(120)
    
def pyramide(cote, n):
    pu()
    for couche in range(n, 0, -1):
        for _ in range(couche):
            pd()
            triangle(cote) 
            pu()
            fd(cote)
        bk(cote*couche); rt(60); fd(cote); lt(60)
    pd()
    
def carre(cote):
 polygoneReg(cote, 4)


def bague(cote):
 for i in range(12):
    if i%2 == 0:
        triangle(cote)
    else:
        carre(cote)
    pu(); fd(cote); pd()
    rt(30)
lt(90)
#triangle(80)
#losange(80)
#hexagone(80)
#cube(80)
#pyramide(16, 5)
#bague(25)

'''
# Solution Exo 4 
1. La fonction polygoneReg est plus générale que triangle car
avec polygoneReg on peut faire le même dessing que triangle, mais
l'inverse n'est pas vrai.
2. Les fonctions polygoneReg et losange ne sont pas comparables.
On ne peut pas se servir de l'une des fonctions pour faire le
même travail que l'autre fonction.
3. Les fonctions triangle et losange ne sont pas comparables.
On ne peut pas se servir de l'une des fonctions pour faire le
même travail que l'autre fonction.

'''

# Solution
def echiquier(n, cote):
 pensize(cote)
 for i in range(n):
    for j in range(n):
        if i%2 != j%2: # tuile noire?
            pu()
            # positionner à la bonne coordonnée
            # avec une tuile blanche en haut à gauche
            goto((j-n/2)*cote, -((i-n/2)*cote+cote/2))
            pd()
            fd(cote)
#echiquier(8, 25)


# Solution Exo 6

def rad(degres):
 # convertir degrés en radians
   return degres*math.pi/180

def aile(longueur):
    # dessiner une aile du moulin à vent
    angle = 30
    fd(longueur*math.cos(rad(angle)))
    rt(90)
    fd(longueur*math.sin(rad(angle)))
    rt(90+angle)
    fd(longueur)
    rt(180-angle)
    
def ailes(longueur, nbAiles):
 # dessiner les ailes du moulin à vent
 for _ in range(nbAiles):
        aile(longueur)
        lt(360/nbAiles)
        
def moulin(hauteur, nbAiles, rotation):
 # dessiner un moulin à vent avec une certaine
 # rotation de ses ailes
    pd()
    pensize(50)
    pencolor(0.5, 0.5, 0.5) # gris
    fd(hauteur)
    pu()
    bk(hauteur*0.1)
    pensize(2)
    pencolor(0,0,0) # noir
    pd()
    rt(rotation)
    ailes(hauteur*0.8, nbAiles)
    rt(-rotation)
    pu()
    bk(hauteur*0.9)
    
def animation():
 rotation = 0
 while True:
        clear()
        pu(); lt(90); bk(100)
        moulin(120, 4, rotation)
        rotation += 0.5
        ht()
        sleep(0.02)
#animation()

# Exo 7  solution :
    # Solution
def animation():
 rotation1 = 0
 rotation2 = 0
 while True:
    clear(); pu()
    bk(80); lt(90); bk(100)
    moulin(120, 3, rotation1)
    rt(90); fd(180); lt(90)
    moulin(90, 4, rotation2)
    rotation1 += 5
    rotation2 += 0.5
    ht()
    sleep(0.02)
animation()

