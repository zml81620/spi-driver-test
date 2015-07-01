all:
	arm-none-linux-gnueabi-gcc spitest.c -o testspi

copy:
	cp testspi /opt/nfsboot/spi/
