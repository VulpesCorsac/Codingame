def encode(message):
    binary = ''.join(format(ord(x), 'b').zfill(7) for x in message)
    to_chuck = ''
    for idx, bit in enumerate(binary):
        if idx >= 1 and binary[idx] == binary[idx-1]:
            to_chuck += '0'
        else: 
            if bit == '1':
                to_chuck += ' 0 0'
            elif bit == '0':
                to_chuck += ' 00 0'
    return to_chuck[1:]


print(encode(input()))