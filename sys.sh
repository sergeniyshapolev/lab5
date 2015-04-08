sudo insmod sys.ko

sudo echo "1 sec."
sudo echo 1 > /sys/kernel/kobject_timer/interval
sleep 3

sudo echo "2 sec."
sudo echo 2 > /sys/kernel/kobject_timer/interval
sleep 6

sudo echo "4 sec."
sudo echo 4 > /sys/kernel/kobject_timer/interval
sleep 8

sudo rmmod sys

