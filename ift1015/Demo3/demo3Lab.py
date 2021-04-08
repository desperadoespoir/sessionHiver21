
#Exo 6
n1 = int ( input ('nombre #1:'))
n2 = int ( input ('nombre #2: '))

if n1 <= 0 or n2 <= 0:
   print('erreur')
else :
  nMin = min(n1 , n2 )
  nMax = max(n1 , n2 )
  if nMax % nMin == 0:
    print ('multiples facteur =', nMax // nMin )
  else :
     print ('pas multiples')
        
        
        # Exo 7 
n = 10
while n >= 1:
  print (n)
  n = n -1    # n--
    
    # exo 8 
n = 1
while n > 10:
 print (n*n )
 n = n +1

    # exo 9
i = 1
N = 16
while i* i <= N:
 print (i)
 i = i + 1 # ou i += 1


#exo 10 
n = 1
somme = 0
while n <= 1000:
 somme = somme + 1/ n # ou somme += 1/n
 n = n + 1 
print ( somme ) 
#exo 11
n = 0
somme = 0
while somme < 10:
  n = n + 1 # ou n += 1
  somme = somme + 1/ n # ou somme += 1/n
print (n)


#Exo 12
n = 11
chaine ="11 "
#print (n)
while n != 1:
  if n %2 == 0:
    n = n //2 # ou n //= 2 
    chaine += str(n)+" "
  else :
    n = 1 + 3* n
    chaine += str(n)+" "
print(chaine)
                  
