# QEMU创建windows arm64 linux / openwrt

### openwrt官方文档
https://openwrt.org/docs/guide-user/virtualization/qemu
`"C:\Program Files\qemu\qemu-system-aarch64.exe" -m 1024 -smp 2 -cpu cortex-a57 -M virt -nographic -kernel openwrt-19.07.3-armvirt-64-Image-initramfs -drive if=none,file=ubuntu.img,id=hd0 -device virtio-blk-device,drive=hd0 -device virtio-net,netdev=net0 -netdev user,id=net0,net=192.168.1.0/24,hostfwd=tcp:127.0.0.1:1122-192.168.1.1:22 -device virtio-net,netdev=net1 -netdev user,id=net1,net=192.0.2.0/24`

`"C:\Program Files\qemu\qemu-system-aarch64.exe" -m 16G -nographic -smp 8 -cpu cortex-a57 -M virt -drive media=cdrom,file=ubuntu-22.04.3-live-server-arm64.iso,index=2 -boot d -hda  ubuntu20.qcow2 -serial mon:stdio -vnc 127.0.0.1:9000`

`"C:\Program Files\qemu\qemu-img" convert -f qcow2 -O qcow2 ubuntu-22.04-server-cloudimg-arm64.img root-disk.qcow2`

`"C:\Program Files\qemu\qemu-img" resize root-disk.qcow2 150G`

vscode远程链接需要：
`opkg install node bash ca-certificates wget uboot-envtools`

### ubuntu img所在位置
https://cloud-images.ubuntu.com/releases/jammy/release-20231026/

### command
```
"C:\Program Files\qemu\qemu-system-aarch64" -machine virt -cpu cortex-a57 -m 1024 -drive file=ubuntu-23.04-amd64.iso,index=0,media=disk,format=raw -device virtio-net,netdev=net0 -netdev user,id=net0
```
command 命令解析：
https://www.datarelab.com/blog/Technical_literature/562.html