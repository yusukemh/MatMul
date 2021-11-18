default: app

run:
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/ICS632/hw5/activity_2:/home/smpi --user 501:20 henricasanova/ics632_smpi smpirun -np $(np) -platform cluster_crossbar_64.xml -hostfile hostfile_64.txt ./matmul $(N)

app: matmul.c
	docker run --rm -it -v /Users/yusukehatanaka/Desktop/ICS632/hw5/activity_2:/home/smpi --user 501:20 henricasanova/ics632_smpi smpicc matmul.c -o matmul

clean:
	rm -f *.o run