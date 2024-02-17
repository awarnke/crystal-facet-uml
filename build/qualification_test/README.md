
Qualification Tests
=============

Goal
-----------

Qualification tests shall test the integrated software in the intended operation environment.

Strategy
-----------

Manual and automatic test cases shall be performed.
It is expected that many releases will have to be tested,
therefore automatic test cases are preferable.

The software shall be tested on multiple processor architectures and operating systems.
To achieve this, the qemu and wine emulation environments are preferred tools.

How To
-----------

qemu:

Create an empty disk

> qemu-img debian-testing_20GB 20G

To install an OS from an .iso image, you may adapt and use the following script:

> #!/bin/sh
> CD=debian-testing-amd64-netinst.iso
> DISK=debian-testing_20GB.img
> NDEV=e1000
> qemu-system-x86_64 -drive file=${DISK},format=raw,index=0,media=disk -cdrom ${CD} \
> -m 2047M -boot order=dc \
> -netdev user,id=mynet0,net=192.168.178.0/24,dhcpstart=192.168.178.9,hostfwd=tcp::8080-:80 \
> -device ${NDEV},netdev=mynet0

After installing, remove the -cdrom * and -boot * parameters

wine:

> see  [../win/README.md](win)

automated tests:

> see  [./run_q_test.sh](run_q_test)
