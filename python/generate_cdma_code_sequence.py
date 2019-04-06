#!/usr/bin/env python3

import argparse
import hashlib
import struct
import numpy as np


def sha512_sequence(sequence_id, code_length):
    # Stage 1
    sequence_stage_1 = hashlib.sha3_512(sequence_id.encode()).digest()

    # Stage 2
    sequence = bytes()
    # Divide length by 512 because we use sha3_512 for code generation
    for idx in range(int(code_length/512)):
        hex_vals_idx = sequence_stage_1.hex()[0+idx*4:4+idx*4].encode()
        sequence = sequence + hashlib.sha3_512(hex_vals_idx).digest()

    return sequence

def write_header_file(header_file_name, sequence, sequence_id):
    output_file = open(header_file_name, "w")
    prefix = ("#define CDMA_CODE_BYTES " + str(len(sequence)) + "\n\n"
              "// Code sequence ID: " + sequence_id + "\n"
              "const uint8_t cdma_sequence[CDMA_CODE_BYTES] = {\n    ")
    postfix = "\n};"
    output_file.write(prefix)
    for idx in range(len(sequence)):
        output_file.write("0b{:08b}".format(sequence[idx]) + ",")
        columns = 8
        if idx >= columns-1 and idx < len(sequence)-1 and 0 == (idx+1) % columns:
            output_file.write("\n    ")
        else:
            if idx < len(sequence)-1:
                output_file.write(" ")
    output_file.write(postfix)
    output_file.close()

def write_reference_file(reference_file_name, sequence):
    output_file = open(reference_file_name, "wb")
    sequence.tofile(output_file)
    output_file.close()

def main():
    parser = argparse.ArgumentParser()
    required = parser.add_argument_group('required arguments')
    required.add_argument("-i", "--sequence-id", type=str, required=True,
                        help="The id for generating the code sequence")
    parser.add_argument("-l", "--code-length", type=int, default=1024,
                        help="Length of the code (powers of 2, maximum 8192)")
    parser.add_argument("-c", "--header-file", type=str, default="cdma_sequence.h",
                        help="Name of the header file for the tag "
                        "(C header file)")
    parser.add_argument("-r", "--reference-file-bits", type=str,
                        help="Name of the reference file for the receiver "
                        "(bits (cdma chips) stored in bytes)")
    parser.add_argument("-f", "--reference-file-bits-fft", type=str,
                        help="Name of the fft reference file for a fast "
                        "correlation in the receiver (fft of bits (cdma chips) "
                        "in bytes)")

    args = parser.parse_args()
    sequence_id = args.sequence_id
    code_length = args.code_length
    header_file_name = args.header_file
    if args.reference_file_bits is not None:
        reference_file_bits_name = args.reference_file_bits
    else:
        reference_file_bits_name = "cdma_sequence_id_" + sequence_id + ".byte"
    if args.reference_file_bits_fft is not None:
        reference_file_bits_fft_name = args.reference_file_bits_fft
    else:
        reference_file_bits_fft_name = "cdma_sequence_fft_id_" + sequence_id + ".complex"

    sequence = sha512_sequence(sequence_id, code_length)
    print("Length of generated code sequence in bytes:", len(sequence))

    write_header_file(header_file_name, sequence, sequence_id)
    print("Header file written to:", header_file_name)

    # Unpack bytes with 8 bits to single bits per byte
    sequence_uint8 = np.frombuffer(sequence, dtype=np.uint8)
    sequence_unpacked = np.unpackbits(sequence_uint8)

    write_reference_file(reference_file_bits_name, sequence_unpacked)
    print("Reference file (bits) written to:", reference_file_bits_name)

    # Generate FFT reference for fast correlation (also remove DC)
    sequence_unpacked_no_dc = np.array(sequence_unpacked,dtype=np.float32)*2-1
    sequence_fft = np.array(np.fft.fft(sequence_unpacked_no_dc),dtype=np.complex64)
    
    write_reference_file(reference_file_bits_fft_name, sequence_fft)
    print("Reference file (fft) written to:", reference_file_bits_fft_name)

if __name__ == '__main__':
    main()