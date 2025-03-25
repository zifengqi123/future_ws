# 一、 future_ws

听说是个好东西

# 二、 环境配置

1. docker ubuntu 18.04 用于编译系统
```
docker pull ubuntu:bionic

18.04
docker run -t -i -v /home/fengqi/:/home -v /tmp/.X11-unix/:/tmp/.X11-unix/ -e DISPLAY=:0 -v /dev/:/dev/ --privileged=true --name fq_bionic --net=host --device=/dev/dri ubuntu:bionic

docker start fq_bionic

docker exec -it fq_bionic bash
```

2. 依赖安装
```
uname -a

apt update

apt-get install lsb-core libncurses* vim libc6 libc-bin gcc make cmake g++

# lsb_release -a
LSB Version:    core-9.20170808ubuntu1-noarch:security-9.20170808ubuntu1-noarch
Distributor ID: Ubuntu
Description:    Ubuntu 18.04.6 LTS
Release:        18.04
Codename:       bionic

# apt install build-essential bison flex zlib1g-dev libncurses5-dev subversion quilt intltool ruby fastjar zip unzip gawk git-core libz-dev

# apt install binutils bzip2 flex gawk gcc grep libc-dev libz-dev python3 rsync subversion unzip libelf-dev libthread-queue-any-perl xsltproc

# apt install build-essential clang flex bison g++ gawk gcc-multilib g++-multilib gettext git libncurses5-dev libssl-dev python3-setuptools rsync swig unzip zlib1g-dev file wget

# apt install libboost-dev libxml-parser-perl libusb-dev bin86 bcc sharutils gcc-multilib b43-fwcutter 

# apt-cache search openjdk
# apt install openjdk-8-jdk


//apt-get -y install build-essential binutils bzip2 gawk gettext git libncurses5-dev libz-dev patch python3 python2.7 unzip zlib1g-dev lib32gcc1 libc6-dev-i386 subversion flex uglifyjs git-core gcc-multilib p7zip p7zip-full msmtp libssl-dev texinfo libglib2.0-dev xmlto qemu-utils upx libelf-dev autoconf automake libtool autopoint device-tree-compiler g++-multilib antlr3 gperf wget curl swig rsync subversion build-essential libncurses5-dev zlib1g-dev gawk git ccache gettext libssl-dev xsltproc zip make

```

3. 登陆与编译
```
【注：一定要切换用户 su pos】

fengqi@IRL8:~$ docker exec -it fq_bionic bash
root@IRL8:/# su pos
pos@IRL8:~$ cd ~/pos_ws/LYNQ_SDK_ASR_L508_X6_ZY-001_V04.01.02.01
pos@IRL8:~/pos_ws/LYNQ_SDK_ASR_L508_X6_ZY-001_V04.01.02.01$ 
pos@IRL8:~/pos_ws/LYNQ_SDK_ASR_L508_X6_ZY-001_V04.01.02.01$ ./build.sh 

```
