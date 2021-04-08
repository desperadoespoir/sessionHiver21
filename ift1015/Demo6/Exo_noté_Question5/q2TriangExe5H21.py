def triangle(base):
    size = 1
    half = base >> 1
    while size <= base:
        fd(1)
        size += 2
        pensize(size)
    pensize(1)
    
clear()
# ht()
triangle(100) 
    