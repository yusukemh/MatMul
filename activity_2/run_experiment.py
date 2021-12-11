from subprocess import call, check_output

def run_command(command):
    print(command)
    call(command.split())
np = 1
N = 32000
for N in [2000, 3000, 4000, 5000, 6000, 7000, 8000, 16000, 32000]:
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_2/fox {N}")