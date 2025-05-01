out_bits = [119, 96, 14, 123, 128, 140, 136, 148, 145, 158, 154, 167, 163, 160, 173]
constant = {}

with open('training.data', 'r') as f:
    for line in f:
        challenge, response = line.strip().split('|')
        for bit in out_bits:
            if response[bit] == '0':
                if bit not in constant:
                    constant[bit] = list(challenge)
                for i in range(0, len(constant[bit])):
                    if constant[bit][i] != challenge[i]:
                        constant[bit][i] = '_'

out = ["_"]*32
for bit in out_bits:
    print(f"{bit:03d} {''.join(constant[bit])}")
    for i in range(0, len(constant[bit])):
        if constant[bit][i] != '_':
            out[i] = constant[bit][i]
print("".join(out))