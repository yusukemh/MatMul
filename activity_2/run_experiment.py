from subprocess import call, check_output

def run_command(command):
    print(command)
    call(command.split())
np = 100
N = 1600
print("STARTS HERE==============================")
print("=========================================")
# run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {4} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_2/cannon {2000}")
# run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {1} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_2/cannon {2000}")


for N in [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000]:
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {1} -platform platforms/cluster_1600_ideal.xml -hostfile platforms/hostfile_1600.txt ./activity_2/matmul {N}")
    # continue
    # run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_2/matmul {N}")