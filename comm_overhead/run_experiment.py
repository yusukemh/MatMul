from subprocess import call, check_output

def run_command(command):
    print(command)
    call(command.split())
np = 16
N = 1600

print("STARTS HERE####################################")
print()
print()
# run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./comm_overhead/fox {N}")
for N in [8*i for i in range(100, 410, 10)]:
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./comm_overhead/fox {N}")
# normal experiment
#for i in range(4):
#    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/fox {N}")
# ideal platform experiment
for i in range(1):
    continue
    print("====================================")
    print("running outer on realistic flatform")
    print("====================================")
    #run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {4} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/cannon {1600}")
    #run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {4} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/fox {1600}")
    #run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {4} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/matmul {1600}")
for np in [1, 4, 16, 64, 100, 400]:
    continue
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600_ideal.xml -hostfile platforms/hostfile_1600.txt ./activity_1/cannon {N}")