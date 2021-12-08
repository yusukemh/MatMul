default: app

run:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul $(N)

c-64:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./cannon $(N)

f-64:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./fox $(N)

o-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/matmul $(N)

c-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/cannon $(N)

f-1600:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun --cfg=smpi/host-speed:5218505859.375f -np $(np) -platform platforms/cluster_1600.xml -hostfile platforms/hostfile_1600.txt ./activity_1/fox $(N)

app: matmul.c cannon.c fox.c
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc activity_1/matmul.c -o activity_1/matmul
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc activity_1/cannon.c -o activity_1/cannon
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc activity_1/fox.c -o activity_1/fox

clean:
	rm -f *.o run