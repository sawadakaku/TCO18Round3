#!/usr/bin/env python3

import subprocess
import argparse


def argparser():
    parser = argparse.ArgumentParser(
        description='compile source')
    parser.add_argument('source', metavar='source', type=str,
                        help='path to source to be compiled')
    return parser


def main():
    parser = argparser()
    args = parser.parse_args()
    source_path = args.source
    exec_path = source_path.replace('cpp', 'out').replace('code', 'execs')
    # compile c++ source file
    subprocess.check_output(
        ['g++', source_path, '-o',
         exec_path,
         '-g', '-O0',
         '-std=c++11'],
        timeout=3)


if __name__ == '__main__':
    main()