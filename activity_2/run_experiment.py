from subprocess import call, check_output

def run_command(command):
    print(command)
    call(command.split())
np = 1
N = 1600
for np in [1, 4, 16, 64, 100, 400]:
    run_command(f"docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np {np} -platform platforms/cluster_1600_ideal.xml -hostfile platforms/hostfile_1600.txt ./activity_2/fox {N}")