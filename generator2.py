from math import ceil 

MAX = 503625
MAX_THREADS = 16
L, R = '{', '}'

def generate_loop(loop):
    lower, upper = loop[0], min(loop[1], MAX)
    return f"for(j = {lower}; j <= {upper}; j++) _solve(b, j);"

def generate_section(loops):
    loops = [generate_loop(loop) for loop in loops]
    return """
    #pragma omp section
    {
        int j; 
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
    {
    """ + ''.join(sections) + """
    }"""


def generate_code(blocks):
    blocks = [ generate_block(block) for block in blocks ]
    return """
    #pragma omp parallel num_threads(8)
    {
     """ + ''.join(blocks) + """
    }"""


blocks = []
i = [1]
while i[-1] < MAX:
    sections = []
    for j in i:
        sections.append((j, j+9))
    i = [ j+10 for j in i if (j+10) not in i ] 
    if i[-1] > MAX: 
        i = i[:-1]
    if i[-1] + 840 < MAX: 
        i.append(i[-1] + 840)
    blocks.append(sections)

code = generate_code(blocks)

with open("Solver.template", 'r') as f:
    template = f.read()
    open("Solver.cpp", 'w').write(template.replace("<INSERT>", code))