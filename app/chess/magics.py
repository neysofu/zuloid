from random import getrandbits, randrange


def find_file_magics_by_i(file_i):
    while True:
        # Try a random magic.
        source_mask = randrange(1, 0x40) << 1
        source_mask <<= file_i * 8
        # print('Attempting the source mask {}'.format(bin(source_mask)))
        target_mask = randrange(1, 0x40) << 58
        # print('The target mask is {}'.format(bin(target_mask)))
        if target_mask % source_mask == 0:
            magic = int(target_mask / source_mask)
            if bin(source_mask).count('1') != bin(source_mask * magic).count('1'):
                # print('The magic candidate doesn\'t have same bitcnt')
                pass
            else:
                # print('Found a possible magic value: {}'.format(hex(magic)))
                found = True
                # Check with every possible source/target pair.
                for source_mask in range(0, 0x40):
                    source_mask <<= 1
                    source_mask <<= file_i * 8
                    target_mask_s = bin(source_mask * magic)
                    # print('Testing compatibility with {}'.format(hex(source_mask)))
                    if bin(source_mask).count('1') != target_mask_s.count('1'):
                        found = False
                        break
                if found and (0x7e << (file_i * 8)) * magic == 0xfc00000000000000:
                    # print('Tests ok')
                    return magic


def find_rank_magics_by_i(rank_i):
    while True:
        # Try a random magic.
        source_mask = getrandbits(64)
        source_mask &= 0x1010101010100
        source_mask <<= rank_i
        #print('Attempting the source mask {}'.format(bin(source_mask)))
        if source_mask == 0:
            continue
        target_mask = randrange(1, 0x40) << 58
        #print('The target mask is {}'.format(bin(target_mask)))
        if target_mask % source_mask == 0:
            magic = int(target_mask / source_mask)
            if bin(source_mask).count('1') != bin(source_mask * magic).count('1'):
                #print('The magic candidate doesn\'t have same bitcnt')
                pass
            else:
                #print('Found a possible magic value: {}'.format(hex(magic)))
                found = True
                # Check with every possible source/target pair.
                for i in range(0, 1000):
                    source_mask = getrandbits(64)
                    source_mask &= 0x1010101010100
                    source_mask <<= rank_i
                    target_mask_s = bin(source_mask * magic)
                    # print('n {} Testing compatibility with {}'.format(
                    #    i, hex(source_mask)))
                    if bin(source_mask).count('1') != target_mask_s.count('1'):
                        found = False
                # and (0x1010101010100 << rank_i) * magic in range(0xfc00000000000000, 2**65):
                if found:
                    # print('Tests ok')
                    return magic


def find_file_magics():
    print('FILE')
    for i in range(0, 8):
        print('{}\t{}'.format(chr(ord('a') + i), hex(find_file_magics_by_i(i))))
    print('')


def find_rank_magics():
    print('RANK')
    for i in range(0, 8):
        print('{}\t{}'.format(i + 1, hex(find_rank_magics_by_i(i))))
    print('')


if __name__ == '__main__':
    find_file_magics()
    find_rank_magics()
