## 如何debug JIT生成的可执行文件
### 打印出JIT方法所在地址
```C++
auto fun = createJit(program->codegen());
std::cout << "ptr: 0x" << std::hex<< (void*) fun << std::endl;
fun();
```
### 使用GDB命令打印指令
`-exec x/20i 0x....`

### 效果
```
-exec x/40i 0xfffff7ff6000
   0xfffff7ff6000:	stp	x29, x30, [sp, #-16]!
   0xfffff7ff6004:	mov	x29, sp
   0xfffff7ff6008:	sub	sp, sp, #0x10
   0xfffff7ff600c:	mov	x9, #0xf020                	// #61472
   0xfffff7ff6010:	movk	x9, #0xe7b0, lsl #16
   0xfffff7ff6014:	movk	x9, #0xffff, lsl #32
   0xfffff7ff6018:	movk	x9, #0x0, lsl #48
   0xfffff7ff601c:	sub	x10, x29, #0x8
   0xfffff7ff6020:	str	x9, [x10]
   0xfffff7ff6024:	mov	x9, #0x1a80                	// #6784
   0xfffff7ff6028:	movk	x9, #0xaab8, lsl #16
   0xfffff7ff602c:	movk	x9, #0xaaaa, lsl #32
   0xfffff7ff6030:	movk	x9, #0x0, lsl #48
   0xfffff7ff6034:	sub	x10, x29, #0x10
   0xfffff7ff6038:	str	x9, [x10]
   0xfffff7ff603c:	sub	sp, sp, #0x10
   0xfffff7ff6040:	mov	x9, #0x2a                  	// #42
   0xfffff7ff6044:	movk	x9, #0x0, lsl #16
   0xfffff7ff6048:	sub	x10, x29, #0x18
   0xfffff7ff604c:	str	x9, [x10]
   0xfffff7ff6050:	sub	x10, x29, #0x8
   0xfffff7ff6054:	ldr	x9, [x10]
   0xfffff7ff6058:	mov	x0, x9
   0xfffff7ff605c:	mov	x9, #0xfb80                	// #64384
   0xfffff7ff6060:	movk	x9, #0xaaae, lsl #16
   0xfffff7ff6064:	movk	x9, #0xaaaa, lsl #32
   0xfffff7ff6068:	movk	x9, #0x0, lsl #48
   0xfffff7ff606c:	blr	x9
   0xfffff7ff6070:	sub	x10, x29, #0x18
   0xfffff7ff6074:	ldr	x9, [x10]
   0xfffff7ff6078:	mov	x0, x9
   0xfffff7ff607c:	mov	x9, #0xfbd8                	// #64472
   0xfffff7ff6080:	movk	x9, #0xaaae, lsl #16
   0xfffff7ff6084:	movk	x9, #0xaaaa, lsl #32
   0xfffff7ff6088:	movk	x9, #0x0, lsl #48
   0xfffff7ff608c:	blr	x9
   0xfffff7ff6090:	mov	sp, x29
   0xfffff7ff6094:	ldp	x29, x30, [sp], #16
   0xfffff7ff6098:	ret
```