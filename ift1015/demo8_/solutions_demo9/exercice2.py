# Solution

def enCommun(texte1, texte2):
    for c in texte1:
        if texte2.find(c) >= 0:
            return True
    return False

def enCommunTest():
    assert enCommun('x', 'x')
    assert enCommun('albert', 'dubuc')
    assert enCommun('dubuc', 'albert')
    assert not enCommun('', '')
    assert not enCommun('x', '')
    assert not enCommun('', 'x')
    assert not enCommun('albert', 'sunny')
    assert not enCommun('sunny', 'albert')

enCommunTest()
