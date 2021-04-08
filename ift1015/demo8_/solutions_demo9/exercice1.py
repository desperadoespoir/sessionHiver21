# Solution

def compter(texte1, texte2):
    return len(texte1.split(texte2)) - 1

def compterTest():
    assert 5  == compter('abracadabra', 'a')
    assert 2  == compter('abracadabra', 'bra')
    assert 2  == compter('abracadabra', 'abra')
    assert 1  == compter('abracadabra', 'cad')
    assert 0  == compter('abracadabra', 'A')

compterTest()