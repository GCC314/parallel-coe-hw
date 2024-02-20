import subprocess
import re
from functools import lru_cache

def gen_input(caseid, alpha, beta): # generate input file text
    txt = """calculation matadd_mpi
output_to_file output/result.txt
print_mpi_log 1
timer_print 1
matrix_type file
matrix_1 ../matadd/case_1/mat_A.txt
matrix_2 ../matadd/case_1/mat_B.txt
"""
    txt = txt.replace('case_1', f"case_{caseid}")
    txt1 = f"alpha {alpha}"
    txt2 = f"beta {beta}"
    return txt + "\n" + txt1 + "\n" + txt2 + "\n"

def concat(pnum): # concat the separate logs
    matrix = []
    nowsize = [0, 0]
    for id in range(pnum):
        with open(f"./output/process_{id}.log", "r") as fp:
            txtlines = fp.readlines()
            rawsize = txtlines[2]
            rawmat = txtlines[6:]
            msize = list(map(int, re.findall("\d+", rawsize)))
            if(nowsize[1] != 0): assert(msize[1] == nowsize[1])
            else: nowsize[1] = msize[1]
            nowsize[0] += msize[0]
            for rawline in rawmat:
                matline = list(map(float, re.findall("[\d\.]+", rawline)))
                if(len(matline) == 0): continue
                assert(len(matline) == msize[1])
                matrix.append(matline)
    assert(len(matrix) == nowsize[0] and len(matrix[0]) == nowsize[1])
    return matrix

def getresult():    # result.txt
    matrix = []
    nowsize = [0, 0]
    with open(f"./output/result.txt", "r") as fp:
        txtlines = fp.readlines()
        rawsize = txtlines[0] + txtlines[1]
        rawmat = txtlines[2:]
        msize = list(map(int, re.findall("\d+", rawsize)))
        if(nowsize[1] != 0): assert(msize[1] == nowsize[1])
        else: nowsize[1] = msize[1]
        nowsize[0] += msize[0]
        for rawline in rawmat:
            matline = list(map(float, re.findall("[\d\.]+", rawline)))
            if(len(matline) == 0): continue
            assert(len(matline) == msize[1])
            matrix.append(matline)
    assert(len(matrix) == nowsize[0] and len(matrix[0]) == nowsize[1])
    return matrix

def matx(caseid, matname): # from matadd/case_i/mat_x.txt
    matrix = []
    nowsize = [0, 0]
    with open(f"../matadd/case_{caseid}/mat_{matname}.txt", "r") as fp:
        txtlines = fp.readlines()
        rawsize = txtlines[1] + txtlines[3]
        rawmat = txtlines[8:]
        msize = list(map(int, re.findall("\d+", rawsize)))
        if(nowsize[1] != 0): assert(msize[1] == nowsize[1])
        else: nowsize[1] = msize[1]
        nowsize[0] += msize[0]
        for rawline in rawmat:
            matline = list(map(float, re.findall("[\d\.]+", rawline)))
            if(len(matline) == 0): continue
            assert(len(matline) == msize[1])
            matrix.append(matline)
    assert(len(matrix) == nowsize[0] and len(matrix[0]) == nowsize[1])
    return matrix

def std(caseid, alpha, beta):    # standard answer
    A, B = matx(caseid, 'A'), matx(caseid, 'B')
    assert(len(A) == len(B) and len(A[0]) == len(B[0]))
    m, n = len(A), len(A[0])
    Ret = [[0.0 for _ in range(n)] for _ in range(m)]
    for i in range(m):
        for j in range(n):
            Ret[i][j] = alpha * A[i][j] + beta * B[i][j]    
    return Ret

def cmp2(A, B): # compare two matrix
    eps = 1e-3
    assert(len(A) == len(B) and len(A[0]) == len(B[0]))
    m, n = len(A), len(A[0])
    for i in range(m):
        for j in range(n):
            if abs(A[i][j] - B[i][j]) > eps:
                return False
    return True
    

def check(caseid, pnum, alpha, beta):
    C, R, S = concat(pnum), getresult(), std(caseid, alpha, beta)
    return cmp2(C, R) and cmp2(R, S)


@lru_cache
def isfakecase(caseid): # check if matsizes deliberately conflict
    def getsize(caseid, matname):
        with open(f"../matadd/case_{caseid}/mat_{matname}.txt", "r") as fp:
            txtlines = fp.readlines()
            rawsize = txtlines[1] + txtlines[3]
            msize = list(map(int, re.findall("\d+", rawsize)))
            return msize
    sa, sb = getsize(caseid, 'A'), getsize(caseid, 'B')
    return sa[0] != sb[0] or sa[1] != sb[1]

def runtask(caseid, pnum, alpha, beta):
    with open("INPUT.txt", "w") as fp: 
        print(gen_input(caseid, alpha, beta), file=fp)
    ret = subprocess.run(f"make runmpi_ MPI_PROCESS_NUM={pnum} MPIRUN_FLAGS=--oversubscribe", shell=True, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
    if(ret.returncode != 0):
        return "AC" if (isfakecase(caseid)) else "RE"
    return "AC" if check(caseid, pnum, alpha, beta) else "WA"



subprocess.run("make clean", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
subprocess.run("make mpi", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

pset = [1, 2, 4, 8, 16, 32, 3, 7, 13]

abset = [(0.2, 0.6), (0.4, 0.8), (1, 0.3)]

cnt, tot = 0, 0

for i in range(1, 6):
    for p in pset:
        for alpha, beta in abset:
            sta = runtask(i, p, alpha, beta)
            tot += 1
            if sta == "AC": cnt += 1
            print(f"i={i},p={p},a={alpha},b={beta},status={sta}")

print(f"Total:{cnt}/{tot}")