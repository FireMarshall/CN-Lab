# reverse iterator

for color in reversed(colors):
    print(color)

for (name, color) in izip(names, colors):
    print(name,color)

blocks = []

for block in iter(partial(f.read, 32), ''):
    blocks.append(block)

def find(seq, target):
    for i, val in enumerate(seq):
        if val == target:
            break
    else:
        return -1
    return 1

