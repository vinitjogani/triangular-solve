from pickle import load, dump

D = load(open('D.dag', 'rb'))
I = load(open('I.dag', 'rb'))
# D = {}
# I = {}
# print("Generating DAG.")
# with open('af_0_k101.mtx') as f:
#     first = True
#     while True:
#         line = f.readline().strip()
#         if line[0:1] == '%': 
#             continue
#         elif first: 
#             first = False
#             continue
#         elif line == '': 
#             break 

#         r, c = list(map(int, line.split(' ')[:-1]))
        
#         if c not in D: D[c] = []
#         if r not in I: I[r] = []
#         if c != r: 
#             D[c].append(r)
#             I[r].append(c)

#         if c % 10000 == 0: 
#             print("\rRows processed:", c, end='')
#     print()
# dump(D, open('D.dag', 'wb'))
# dump(I, open('I.dag', 'wb'))

explored = {}
layers = []
i = 0
while len(explored) != len(I):
    i += 1
    layer = [
        d for d in I 
        if d not in explored and 
        len([k for k in I[d] if k not in explored]) == 0
    ]
    print(i, len(layer), layer)
    for l in layer:
        explored[l] = True
        # explored.extend(layer)
    layers.append(layer)

for i, layer in enumerate(layers): 
    print(i+1, len(layer))
# print(layers)

# B = []
# print("Processing vector b.")
# with open('b_sparse_af_0_k101.mtx') as f:
#     first = True
#     while True:
#         line = f.readline().strip()
#         if line[0:1] == '%': 
#             continue
#         elif first:
#             first = False
#             parts = line.split(' ')
#             if len(parts) == 2:  
#                 B.extend(range(int(parts[0])))
#                 break
#             continue
#         if line == '': 
#             break 

#         r = int(line.split(' ')[0])
#         B.append(r)
# print(B)

# print("Performing DFS.")
# explored = {}
# frontier = list(B)
# while len(frontier):
#     v = frontier.pop()
#     if v not in explored:
#         explored[v] = True
#         frontier.extend(D[v])

#     if len(explored) % 1000 == 0: 
#         print("\rNodes explored:", len(explored), end='')

# reachSet = sorted(explored.keys())
# print("\nReach-set size is", len(reachSet))


# with open("Solver.template") as f:
    # template = f.read()
    
    # out = f"int p, j, px, reachSetSize={len(reachSet)};\n"
    # out += "int reachSet[] = {" + ','.join(map(str, reachSet)) + "};"
    # last_peel = -1
    # acc = 0
    # old_v = 1
    # l, r = '{', '}'
    # for i, v_ in enumerate(reachSet):
    #     acc += 1
    #     for intermediate in range(old_v, v):
    #         acc += len(D[intermediate])
    #     v = v_ - 1
    #     if len(D[v_]) >= 24:
    #         if last_peel < i-1:
    #             out +=  f"""
    #             for (px={last_peel+1}; px < {i}; px++) {l}
    #                 j = reachSet[px]; b[j] /= val[ptr[j]];
    #                 for (p = ptr[j]+1; p < ptr[j+1]; p++)
    #                     b[ind[p]] -= val[p] * b[j];
    #             {r}"""

    #         out +=  f"""
    #             b[{v}] /= val[{acc-1}];
    #             for (p = {acc}; p < {acc+len(D[v_])}; p++)
    #                 b[ind[p]] -= val[p] * b[{v}];"""
    #         last_peel = i
    #     acc += len(D[v_])
    #     old_v = v

    #     if v_ % 10000 == 0:
    #         status = round(i*100/len(reachSet))
    #         progress_bar = '=' * status
    #         if status < 100: progress_bar = progress_bar[:-1] + '>'
    #         progress_bar += ' ' * (100-status)
    #         print("\rGenerating code: [", progress_bar, '] ', status , end='%')

    # out +=  f"""
    #     for (px={last_peel+1}; px < reachSetSize; px++) {l}
    #         j = reachSet[px];  b[j] /= val[ptr[j]];
    #         for (p = ptr[j]+1; p < ptr[j+1]; p++)
    #             b[ind[p]] -= val[p] * b[j];
    #     {r}"""
    # out = out.replace('        ', '')
    # print('\n\n', len(out))
    # open("Solver.cpp", 'w').write(template.replace("<INSERT>", out))