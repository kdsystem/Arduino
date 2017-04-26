git clone https://github.com/stanleyseow/RF24.git
cd RF24
cd librf24-rpi/librf24
make
sudo make install

g++ -Wall -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -L/mnt/project/lib/RF24/ -lrf24-bcm transfer.cpp -o transfer
