import pprint
GIVEN = (
    (
        "X ",
        "##",
        "##",
        "# ",
    ),
    (
        "X##",
        "## ",
        "#  ",
    ),
    (
        " X ",
        "## ",
        "###",
    ),
    (
        " X",
        " #",
        "##",
        "##",
    ),
    (
        "X  ",
        "#  ",
        "#  ",
        "###",
    ),
    (
        "X",
        "#",
        "#",
        "#",
        "#",
        "#",
    ),
    (
        "X###",
        "#  #",
    ),
    (
        "X# ",
        "#  ",
        "###",
    ),
    (
        "X# ",
        " # ",
        " # ",
        " ##",
    ),
    (
        "X #",
        "###",
        "#  ",
    ),
)

SHAPES = []
for s, shape in zip(('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'), GIVEN):
    mp = {' ': 0, '#': 1, 'X': 2}
    shape = tuple([tuple([mp[l]  for l in row]) for row in shape])
    for r in range(4): # Rotations
        mark = (0, 0)
        for h, row in enumerate(shape):
            for w, cell in enumerate(row):
                if cell == 2:
                    mark = (w, h)

        SHAPES.append((s, r, mark, shape))
        shape = list(zip(*shape[::-1]))
