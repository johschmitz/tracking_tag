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
        hex_val_idx = code_stage_1.hex()[idx].encode()
        code = code + hashlib.sha3_512(hex_val_idx).digest()

    return code

def write_output_file(output_file_name, code):
    output_file = open(output_file_name, "w")
    prefix = ("#define CDMA_CODE_BYTES " + str(len(code)) + "\n\n"
              "uint8_t cdma_code[CDMA_CODE_BYTES] = {\n    ")
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

def main():
    parser = argparse.ArgumentParser()
    required = parser.add_argument_group('required arguments')
    required.add_argument("-i", "--code-id", type=str, required=True,
                        help="The id for generating the code")
    parser.add_argument("-l", "--code-length", type=int, default=1024,
                        help="Length of the code")
    parser.add_argument("-o", "--output-file", type=str, default="cdma_code.h",
                        help="Name of the output file (C header file)")
    args = parser.parse_args()
    code_id = args.code_id
    code_length = args.code_length
    output_file_name = args.output_file

    code = sha512_code(code_id, code_length)
    write_output_file(output_file_name, code)


if __name__ == '__main__':
    main()