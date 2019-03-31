#!/usr/bin/env python3

import argparse
import hashlib


def sha512_code(code_id, code_length):
    # Stage 1
    code_stage_1 = hashlib.sha3_512(code_id.encode()).digest()

    # Stage 2
    code = bytes()
    # Divide length by 512 because we use sha3_512 for code generation
    for idx in range(int(code_length/512)):
        hex_vals_idx = code_stage_1.hex()[0+idx*4:4+idx*4].encode()
        code = code + hashlib.sha3_512(hex_vals_idx).digest()

    return code

def write_header_file(header_file_name, code, code_id):
    output_file = open(header_file_name, "w")
    prefix = ("#define CDMA_CODE_BYTES " + str(len(code)) + "\n\n"
              "// Code ID: " + code_id + "\n"
              "const uint8_t cdma_code[CDMA_CODE_BYTES] = {\n    ")
    postfix = "\n};"
    output_file.write(prefix)
    for idx in range(len(code)):
        output_file.write("0b{:08b}".format(code[idx]) + ",")
        columns = 8
        if idx >= columns-1 and idx < len(code)-1 and 0 == (idx+1) % columns:
            output_file.write("\n    ")
        else:
            if idx < len(code)-1:
                output_file.write(" ")
    output_file.write(postfix)
    output_file.close()

def write_reference_file(reference_file_name, code):
    output_file = open(reference_file_name, "wb")
    output_file.write(code)
    output_file.close()

def main():
    parser = argparse.ArgumentParser()
    required = parser.add_argument_group('required arguments')
    required.add_argument("-i", "--code-id", type=str, required=True,
                        help="The id for generating the code")
    parser.add_argument("-l", "--code-length", type=int, default=1024,
                        help="Length of the code (powers of 2, maximum 8192)")
    parser.add_argument("-c", "--header-file", type=str, default="cdma_code.h",
                        help="Name of the header file for the tag (C header file)")
    parser.add_argument("-r", "--reference-file", type=str,
                        help="Name of the reference file for the receiver (raw bytes)")

    args = parser.parse_args()
    code_id = args.code_id
    code_length = args.code_length
    header_file_name = args.header_file
    if args.reference_file is not None:
        reference_file_name = args.reference_file
    else:
        reference_file_name = "cdma_code_id_" + code_id + ".byte"

    code = sha512_code(code_id, code_length)
    print("Length of generated code in bytes:", len(code))
    write_header_file(header_file_name, code, code_id)
    print("Write header file to:", header_file_name)
    write_reference_file(reference_file_name, code)
    print("Write reference file to:", reference_file_name)


if __name__ == '__main__':
    main()