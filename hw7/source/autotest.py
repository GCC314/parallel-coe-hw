import subprocess
import os

DIAGO_DATA_DIR = "./../diago/"
MATMUL_DATA_DIR = "./../matmul/"

DIAGO_CASES = [f'case_{i}' for i in [1, 2, 3]]
MATMUL_CASES = [f'case_{i}' for i in [1, 2, 3, 4, 5]]

TEST_OUTPUT_DATA_DIR = "./../test/"

def Gen_input_file(type, src_dir, resp = 0):
    with open("tmp.in", "w") as fp:
        print("calculation", type, file = fp)
        print(f"result_print {resp}", file = fp)
        print("timer_print 1", file = fp)
        print("matrix_type file", file = fp)
        print("matrix_1", src_dir + "/mat_1.txt", file = fp)
        if(type == "matmul"): 
            print("matrix_2", src_dir + "/mat_2.txt", file = fp)

for case in DIAGO_CASES:
    Gen_input_file("rsmdiago", DIAGO_DATA_DIR + case, resp=1)
    ret = subprocess.run("./main tmp.in", shell = True, stdout=subprocess.PIPE)
    data = ret.stdout.decode()
    
    print("diago", case, "return =", ret.returncode)
    with open(TEST_OUTPUT_DATA_DIR + "diago" + case + ".out", "w") as fp:
        print(data, file = fp)
    
for case in MATMUL_CASES:
    Gen_input_file("matmul", MATMUL_DATA_DIR + case)
    ret = subprocess.run("./main tmp.in", shell = True, stdout=subprocess.PIPE)
    data = ret.stdout.decode()
    
    print("matmul", case, "return =", ret.returncode)
    with open(TEST_OUTPUT_DATA_DIR + "matmul" + case + ".out", "w") as fp:
        print(data, file = fp)
    