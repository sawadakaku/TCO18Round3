#!/usr/bin/env python3

import subprocess
import argparse
import numpy as np
import pandas as pd


seeds = [i for i in range(10)]


def argparser():
    parser = argparse.ArgumentParser(
        description='compile source and test with some cases')
    parser.add_argument('source', metavar='source', type=str,
                        help='path to source to be compiled')
    return parser


def main():
    parser = argparser()
    args = parser.parse_args()
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
             '-exec', '{}'.format(source_path.replace('cpp', 'out')),
             '-seed', f'{seed}'],
            timeout=3).decode('utf-8')
        score = out_data.splitlines()[-1]
        score = int(float(score.split()[-1]))
        d = {'seed': seed, 'score': score}
        df = df.append(d, ignore_index=True)
    df = df.astype(np.uint32)
    csv_path = source_path.replace('cpp', 'csv').replace('code', 'output')
    df.to_csv(f'{csv_path}',
              index=False,
              )


if __name__ == '__main__':
    main()