default: app

run:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul $(N)

c-64:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./cannon $(N)

f-64:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./fox $(N)

o-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform cluster_1600.xml -hostfile hostfile_1600.txt ./matmul $(N)

c-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform cluster_1600.xml -hostfile hostfile_1600.txt ./cannon $(N)

f-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform cluster_1600.xml -hostfile hostfile_1600.txt ./fox $(N)

app: matmul.c cannon.c fox.c
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc matmul.c -o matmul
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc cannon.c -o cannon
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc fox.c -o fox

clean:
	rm -f *.o run