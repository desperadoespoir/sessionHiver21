# Solution

def readFile(path):
    return open(path, 'rb').read().decode('utf-8')

def writeFile(path, texte):
    f = open(path, 'wb')
    f.write(texte.encode('utf-8'))
    f.close()

def justifier(path):

    contenu = readFile(path)
    phrases = contenu.split('.')

    for i in range(len(phrases)):
        phrases[i] = phrases[i].replace('\n', ' ')

    writeFile(path+'.just', '.\n'.join(phrases))

justifier('phrases.txt')    
