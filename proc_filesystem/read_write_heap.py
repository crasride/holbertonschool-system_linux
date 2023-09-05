#!/usr/bin/python3
'''
Script that finds and replaces a string in the heap of a running process.
'''

import sys


# Función para encontrar la dirección del heap en el proceso especificado.
def find_heap_address(pid):
    maps_path = f"/proc/{pid}/maps"
    heap_info = None

    with open(maps_path) as maps:
        for line in maps:
            if "[heap]" in line:
                parts = line.split()
                # Parsea las partes relevantes del archivo /proc/pid/maps.
                start, end = map(lambda x: int(x, 16), parts[0].split('-'))
                mode = parts[1]
                offset = parts[2]
                device = parts[3]
                inode = parts[4]
                # Almacena la información del heap en un diccionario.
                heap_info = {
                    "start": start,
                    "end": end,
                    "mode": mode,
                    "offset": offset,
                    "device": device,
                    "inode": inode
                }
                break

    return heap_info


# Función para reemplazar una cadena en el heap del proceso especificado.
def replace_string_in_heap(pid, search_string, replace_string):
    mem_path = f"/proc/{pid}/mem"
    heap = find_heap_address(pid)

    if heap is None:
        print("Heap not found")
        return

    print(":Found [heap]:")
    print(f"\taddresses = {heap['start']:X}-{heap['end']:X}")
    print(f"\tpermissions = {heap['mode']}")
    print(f"\toffset = {heap['offset']}")
    print(f"\tdevice = {heap['device']}")
    print(f"\tinode = {heap['inode']}")
    print(f"Addr start [{heap['start']:X}] | end [{heap['end']:X}]")

    if not replace_string:
        replace_string = ' ' * len(search_string)

    with open(mem_path, 'rb+') as mem:
        mem.seek(heap['start'])
        heap_memory = mem.read(heap['end'] - heap['start'])

        try:
            i = heap_memory.index(search_string.encode("ASCII"))
            print(f"Found '{search_string}' at {i:X}")
        except ValueError:
            print("No matching string found")
            return

        mem.seek(heap['start'] + i)
        mem.write(replace_string.encode("ASCII"))
        print(f"Writing '{replace_string}' at {heap['start'] + i:X}")


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: {} pid search_string replace_string".format(sys.argv[0]))
        sys.exit(1)

    pid, search, replace = sys.argv[1:]

    if not pid or not search:
        print("Missing value")
        sys.exit(1)

    replace_string_in_heap(pid, search, replace)
