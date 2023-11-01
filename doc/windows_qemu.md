# QEMU创建windows arm64 linux / openwrt

### openwrt官方文档
https://openwrt.org/docs/guide-user/virtualization/qemu

### command
```
"C:\Program Files\qemu\qemu-system-aarch64" -machine virt -cpu cortex-a57 -m 1024 -drive file=ubuntu-23.04-amd64.iso,index=0,media=disk,format=raw -device virtio-net,netdev=net0 -netdev user,id=net0
```