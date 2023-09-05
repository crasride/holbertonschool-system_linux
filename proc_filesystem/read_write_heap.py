#!/usr/bin/python3
'''
Script that finds and replaces a string in the heap of a running process.
'''
import sys


def print_usage():
    print("Usage: {} [PID] search_string replace_string".format(sys.argv[0]))
    sys.exit(1)


def read_process_memory(pid):
    maps_file = "/proc/{}/maps".format(pid)
    mem_file = "/proc/{}/mem".format(pid)

    try:
        with open(maps_file, 'r') as maps:
            for line in maps:
                if "[heap]" in line:
                    start, end = map(lambda x: int(x, 16), line.split()[0].split("-"))
                    print("Found [heap]:")
                    print("    addresses =", line.split()[0])
                    print("    permissions =", line.split()[1])
                    print("    offset =", line.split()[2])
                    print("    device =", line.split()[3])
                    print("    inode =", line.split()[4])
                    print("    Addr start [{}] | end [{}]".format(line.split()[0].split("-")[0], line.split()[0].split("-")[1]))
                    return start, end
    except IOError as e:
        print("[ERROR] Can't open file {}: I/O error({}): {}".format(maps_file, e.errno, e.strerror))
    return None, None


def find_and_replace_string(pid, search_string, replace_string):
    start, end = read_process_memory(pid)

    if start is None or end is None:
        return

    try:
        with open("/proc/{}/mem".format(pid), 'rb+') as mem:
            mem.seek(start)
            heap = mem.read(end - start)
            offset = heap.find(search_string.encode("ASCII"))

            if offset != -1:
                print("Found '{}' at {}".format(search_string, hex(start + offset)))
                print("Writing '{}' at {}".format(replace_string, hex(start + offset)))
                mem.seek(start + offset)
                mem.write(replace_string.encode("ASCII") + b'\0')
            else:
                print("Can't find '{}' in the heap.".format(search_string))
    except IOError as e:
        print("[ERROR] Can't open memory file for PID {}: I/O error({}): {}".format(pid, e.errno, e.strerror))


def main():
    if len(sys.argv) != 4:
        print_usage()

    pid = int(sys.argv[1])
    search_string = sys.argv[2]
    replace_string = sys.argv[3]

    if not search_string or not replace_string:
        print_usage()

    find_and_replace_string(pid, search_string, replace_string)


if __name__ == "__main__":
    main()
