from math import ceil 

MAX = 503625
MAX_THREADS = 16
L, R = '{', '}'

def generate_loop(loop):
    lower, upper = loop[0], min(loop[1], MAX)
    out = ""
    return f"for(j = {lower-1}; j < {upper}; j++) _solve(b, j);"

def generate_section(loops):
    loops = [generate_loop(loop) for loop in loops]
    return """
    #pragma omp section
    {
        """ + '\n'.join(loops) + """
    }"""

def generate_block(block):
    if len(block) > MAX_THREADS:
        sections = []
        for i in range(ceil(len(block) / MAX_THREADS)):
            sections.append(generate_section(block[i*MAX_THREADS:(i+1)*MAX_THREADS]))
    else:
        sections = [ generate_section([sec]) for sec in block ]
    return """
    #pragma omp sections
    { """ + ''.join(sections) + """
    }"""


def generate_code(blocks):
    blocks = [ generate_block(block) for block in blocks ]
    return """
    #pragma omp parallel 
    { 
        int j;
        """ + ''.join(blocks) + """
    }"""


blocks = []
i = [1]
explored = {}
while i[-1] < MAX:
    sections = []
    for j in i:
        explored[j] = True
        sections.append((j, j+9))

    i = [ j+10 for j in i if (j+10) not in explored ]
    if len(i) == 0: break 
    if i[-1] > MAX: i = i[:-1]
    if i[-1] + 840 < MAX and (i[-1] + 840) not in explored: 
        i.append(i[-1] + 840)
    
    blocks.append(sections)

print(blocks[-1][-1])

code = generate_code(blocks)

with open("Solver.template", 'r') as f:
    template = f.read()
    open("Solver.cpp", 'w').write(template.replace("<INSERT>", code))