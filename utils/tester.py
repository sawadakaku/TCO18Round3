#!/usr/bin/env python3

import sys
import os
import glob
import subprocess
import argparse
import numpy as np
import pandas as pd


seeds = [i for i in range(10)]


def argparser():
    parser = argparse.ArgumentParser(description='compile source and test with some cases')
    parser.add_argument('source', metavar='source', type=str, nargs=1,
                        help='path to source to be compiled')
    return parser


def main():
    args = argparser()
    source_path = args.source
    # compile c++ source file
    subprocess.check_output(
        ['g++', source_path, '-o',
         source_path.replace('cpp', 'out'),
         '-g', '-O0',
         '-std=c++11'],
        timeout=3)
    # test with some test cases
    df = pd.DataFrame(columns=['seed', 'score'], dtype=np.uint32)
    for seed in seeds:
        out_data = subprocess.check_output(
            ['java', '-jar', './tester.jar',
             '-exec', '"{}"'.format(source_path.replace('cpp', 'out')),
             '-seed', f'{seed}'],
            timeout=3)
        score = out_data.split_lines()[-1]
        score = int(score.split()[-1])
        d = {'seed': seed, 'score': score}
        df.append(d)


if __name__ == '__main__':
    main()