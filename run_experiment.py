from subprocess import call, check_output

def run_command(command):
    print(command)
    call(command.split())
np = 1
N = 1600
for i in range(4):
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform cluster_1600.xml -hostfile hostfile_1600.txt ./matmul {N}")