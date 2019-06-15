#!/bin/bash

function try2install {
	dpkg -l | grep "ii  $1" > /dev/null
    	if [ $? -eq 0 ]; then 
	    	echo -e "\033[32m $1 \t\t installed! \033[0m"
	else 
		echo -e "\034[31m try to install $1, please input your password or press ENTER! \033[0m"
	    	sudo apt-get -y install $1
		if [ $? -ne 0 ]; then
			echo -e "\033[31m Fail to install $1, try again! \033[0m"
			exit 1
		fi
	fi
}

function install_customer_depdents {
try2install libboost-all-dev
try2install libopencv-dev
try2install libhdf5-dev
try2install libgflags-dev
try2install libgoogle-glog-dev
try2install libleveldb-dev
try2install liblmdb-dev
try2install libsnappy-dev
}

function install_developer_depdents {
install_customer_depdents

try2install protobuf-compiler
try2install libprotobuf-dev
}

## program starts here
if ! [ -z $1 ] && [ $1 = "dev" ]
then
    echo -e "try to install the deb packets for developers!"
    install_developer_depdents 
else
    echo -e "try to install the deb packets for customers!"
    install_customer_depdents
fi
