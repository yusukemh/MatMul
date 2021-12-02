default: app

run:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul $(N)

c:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./cannon $(N)

f:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./fox $(N)

app: matmul.c cannon.c fox.c
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc matmul.c -o matmul
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc cannon.c -o cannon
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/github/MatMul:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc fox.c -o fox

clean:
	rm -f *.o run