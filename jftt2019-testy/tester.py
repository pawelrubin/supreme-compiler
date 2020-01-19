import subprocess

tests_dir = "jftt2019-testy"
tests = [
    ("program0.imp", ([1345601,], [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1],)),
    ("program1.imp", ([], [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97])),
    ("program2.imp", ([12345678901], [857, 1, 14405693, 1])),
    ("program2.imp", ([12345678903], [3, 1, 4115226301, 1])),
    ("0-div-mod.imp", ([1, 0], [1, 0, 0, 0])),
    ("00-div-mod.imp", ([33, 7], [4, 5, -5, -2, 4, -5, -5, 2])),
    ("1-numbers.imp", ([20], [0, 1, -2, 10, -100, 10000, -1234567890, 35, 15, -999, -555555555, 7777, -999, 11, 1, 7777])),
    ("2-fib.imp", ([1], [121393])),
    ("3-fib-factorial.imp", ([20], [2432902008176640000, 6765])),
    ("4-factorial.imp", ([20], [2432902008176640000])),
    ("5-tab.imp", ([], [0, 24, 46, 66, 84, 100, 114, 126, 136, 144, 150, 154, 156, 156, 154, 150, 144, 136, 126, 114, 100, 84, 66, 46, 24, 0])),
    ("6-mod-mult.imp", ([1234567890, 1234567890987654321, 987654321], [674106858])),
    ("7-loopiii.imp", ([0, 0, 0], [31000, 40900, 2222010])),
    ("7-loopiii.imp", ([1, 0, 2], [31001, 40900, 2222012])),
    ("8-for.imp", ([12, 23, 34], [507, 4379, 0])),
    ("9-sort.imp", ([], [1, 14, 12, 7, 6, 15, 3, 19, 13, 21, 18, 22, 9, 11, 16, 17, 8, 20, 4, 10, 2, 5, 1234567890, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,],)),
]

fails = 0

for i, test in enumerate(tests):
    subprocess.run(["../compiler", test[0], f"/tmp/test_{i}"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    args = "\\n".join(str(x) for x in test[1][0]) + "\\n" if len(test[1][0]) > 0 else ""
    test_vm = subprocess.Popen(["./test_vm", f"/tmp/test_{i}"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cout, cerr = test_vm.communicate(input=args.encode("utf-8"))
    results = cout.decode("utf-8").split("\n")[:-2]
    cost = cout.decode("utf-8").split("\n")[-2]
    status = "PASS" if results == list(str(x) for x in test[1][1]) else "FAIL"
    print(test[0], status, "cost", cost, results if status == "FAIL" else "")
    if status == "FAIL":
        fails += 1
    
print(f"{fails=}")
if (fails > 4):
    print("\033[91m" + "FAILING!!!")
    exit(1)
else:
    print("\033[92m" + "GOOD JOB!!!")
    exit(0)

