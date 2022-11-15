import sys

I2C_BUFFER_SIZE = 32

def read(count):
    packet_size = count

    cargo_remaining = packet_size
    read_size = 0
    cargo_read_amount = 0
    first_read = True
    total = 0

    while cargo_remaining > 0:
        if first_read:
            read_size = min(I2C_BUFFER_SIZE, cargo_remaining)
        else:
            read_size = min(I2C_BUFFER_SIZE, cargo_remaining + 4)
        
        print("Read ", read_size)

        if first_read:
            cargo_read_amount = read_size
            first_read = False
        else:
            cargo_read_amount = read_size - 4

        total += cargo_read_amount
        cargo_remaining -= cargo_read_amount

    print("total read: ", total)
            



if __name__ == "__main__":
    read(int(sys.argv[1]))
