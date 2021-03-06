#!/usr/bin/env python3

import argparse
import hashlib
import struct
import numpy as np
import matplotlib.pyplot as plt

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

def balance_sequence(sequence):
    sequence_unpacked = np.unpackbits(np.frombuffer(sequence, dtype=np.uint8))
    # Calculate imbalance between 0s and 1s
    imbalance = sum(sequence_unpacked)-len(sequence_unpacked)/2
    print("Sequence imbalance:",imbalance)
    if imbalance != 0:
        flip_distance = np.floor(len(sequence_unpacked)/2/abs(imbalance))
        print("To balance will flip bits of same type at distance:",flip_distance)
        sequence_balanced = np.ndarray(shape=sequence_unpacked.shape, dtype=np.uint8)
        zero_counter = 0
        one_counter = 0
        flip_counter = 0
        for idx, bit in enumerate(sequence_unpacked):
            # Begin with original sequence
            sequence_balanced[idx] = bit
            # Flip some bits
            if flip_counter < abs(imbalance):
                if imbalance < 0:
                    if 0 == bit:
                        zero_counter += 1
                        if 0 == (zero_counter % flip_distance):
                            sequence_balanced[idx] = 1
                            flip_counter += 1
                else:
                    if 1 == bit:
                        one_counter += 1
                        if 1 == (one_counter % flip_distance):
                            sequence_balanced[idx] = 0
                            flip_counter += 1
        imbalance = sum(sequence_balanced)-len(sequence_balanced)/2
        print("New sequence imbalance:",imbalance)
        sequence_repacked = np.packbits(sequence_balanced.astype(int))
        return sequence_repacked
    else:
        return sequence

def write_header_file(header_file_name, sequence, sequence_id):
    output_file = open(header_file_name, "w")
    prefix = ("#define DATA_BYTES 1\n"
              "#define CDMA_CODE_BYTES " + str(len(sequence)) + "\n"
              "\n"
              "// Data sequence for synchronization\n"
              "const uint8_t data_sequence[DATA_BYTES] = {\n"
              "    0b10101010,\n"
              "};\n"
              "\n"
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

def resample(sequence, chip_rate, rx_sample_rate):
    # Calculate length of resampled sequence
    len_resampled = int(len(sequence)*rx_sample_rate/chip_rate)
    # In the following make it a sum of powers of 2 length
    max_exponent = int(np.log2(len_resampled))
    # FIXME make the smallest power 2 exponent configurable
    min_exponent = 4
    len_power_two = 2**int(max_exponent)
    for exponent in range(max_exponent,min_exponent-1,-1):
        if len_power_two + 2**exponent <= len_resampled:
            len_power_two = len_power_two + 2**exponent
    t_c = 1/chip_rate
    t_s = 1/rx_sample_rate
    t = 0
    sequence_resampled = np.zeros(len_power_two)
    for out_idx in range(min(len_power_two,len_resampled)):
        in_idx = np.int(t/t_c)
        sequence_resampled[out_idx] = sequence[in_idx]
        t = t + t_s
    return sequence_resampled

def main():
    parser = argparse.ArgumentParser(add_help=False)
    required = parser.add_argument_group('required arguments')
    optional = parser.add_argument_group('optional arguments')
    required.add_argument("-i", "--sequence-id", type=str, required=True,
                          help="The id for generating the code sequence.")
    optional.add_argument("-h", "--help", action="help", help="Show this help message"
                          " and exit.")
    optional.add_argument("-l", "--code-length", type=int, default=1024,
                          help="Length of the code (powers of 2, default 1024, maximum 8192)")
    optional.add_argument("-r", "--chip-rate", type=float, default=1.0,
                          help="Chip rate of the transmitter, default: 1.0")
    optional.add_argument("-o", "--rx-sample-rate", type=float, default=1.0,
                          help="Receiver sample rate for the reference code sequence, default: 1.0")
    optional.add_argument("-c", "--header-file", type=str, default="cdma_sequence.h",
                          help="Name of the header file for the tag "
                          "(C header file), default: cdma_sequence.h")
    optional.add_argument("-t", "--reference-file-bits", type=str,
                          help="Name of the reference file for the receiver "
                          "(bits (cdma chips) stored in bytes), default: cdma_sequence_id_<ID>.byte")
    optional.add_argument("-f", "--reference-file-bits-fft", type=str,
                          help="Name of the fft reference file for a fast "
                          "correlation in the receiver (fft of bits (cdma chips) "
                          "in bytes), default: cdma_sequence_fft_id_<ID>.complex")
    optional.add_argument("--fft-input-padding", action='store_true', default=False,
                          help="Zero pad fft input for non circular cross correlation.")
    optional.add_argument("--debug-plot", action='store_true', default=False,
                          help="Show a plot of the correlation for debugging.")

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
    print("Length of generated code sequence:", 8*len(sequence))

    # Make sure we have same number of 0s and 1s
    sequence_balanced = balance_sequence(sequence)

    write_header_file(header_file_name, sequence_balanced, sequence_id)
    print("Header file written to:", header_file_name)

    # Unpack bytes with 8 bits to single bits per byte
    sequence_unpacked = np.unpackbits(np.frombuffer(sequence_balanced, dtype=np.uint8))

    if 1 != args.rx_sample_rate or 1 != args.chip_rate:
        print("Resample reference sequence with factor:", args.rx_sample_rate/args.chip_rate)
        sequence_unpacked = resample(sequence_unpacked, args.chip_rate, args.rx_sample_rate)
        # Go back to uint8
        sequence_unpacked = np.array(sequence_unpacked, dtype=np.uint8)

    write_reference_file(reference_file_bits_name, sequence_unpacked)
    print("Reference file (bits) written to:", reference_file_bits_name)

    # Generate FFT reference for fast correlation (also remove DC)
    sequence_unpacked_no_dc = np.array(sequence_unpacked,dtype=np.float32)*2-1
    if args.fft_input_padding:
        print("Padding fft input with zeros for non-circular fast cross correlation.")
        # Pad fft input with zeros, we have a window of two times sequence length and a shift of +-sequence length (->3)
        fft_in_ref = np.concatenate((sequence_unpacked_no_dc, \
            np.zeros(len(sequence_unpacked)*2)))
    else:
        fft_in_ref = sequence_unpacked_no_dc
    sequence_fft = np.array(np.fft.fft(fft_in_ref),dtype=np.complex64)

    write_reference_file(reference_file_bits_fft_name, sequence_fft)
    print("Reference file (FFT) written to:", reference_file_bits_fft_name)
    print("Complex samples in FFT reference file:",len(sequence_fft))

    if args.debug_plot:
        # Plot for debugging
        test_input_fft = np.fft.fft(np.concatenate((
            np.zeros(len(sequence_unpacked_no_dc)),
            np.random.randint(2,size=len(sequence_unpacked_no_dc)//2)*2-1,
            sequence_unpacked_no_dc,
            np.random.randint(2,size=len(sequence_unpacked_no_dc)//2)*2-1)))
        if args.fft_input_padding:
            corr = np.fft.ifft(test_input_fft*np.conjugate(sequence_fft)) \
                   /len(sequence_unpacked_no_dc)
        else:
            corr = np.fft.ifftshift(np.fft.ifft(sequence_fft*np.conjugate(sequence_fft))) \
                   /len(sequence_unpacked_no_dc)
        plt.plot(corr.real)
        plt.show()

if __name__ == '__main__':
    main()
