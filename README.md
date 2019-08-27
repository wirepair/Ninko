# Ninko 
Ninko is a PIN tool created to aid in instrumenting x86 obfuscated code. It works by logging only calls, writes specified for an exact range. This helps in limiting the amount of information you need to wade through to understand how a binary may or may not be working.

# Unsupported
x64 :( Feel free to submit patches!

#Usage

Ninko works off of JSON formatted configuration files. There are a few things you must know about your binary before you can begin.

## Ninko Configuration Fields
Note "calls" can mean any type, direct or indirect, jmp, jz, call, ret etc.

* "image_name" - The image name: One would hope you'd know the name of the executable/dll you wish to monitor.
* "outfile" - Where to save the results to. 
* "hook_functions" - Whether or not to hook GetProcAddress or any hooks you add to the source.
* "disable_log_calls" - true = Do not log calls, false = log them.
* "disable_log_writes" - true = Do not log writes, false = log them.
* "disable_log_reads" - true = Do not log reads, false = log them.
* "ignore_internal_calls" - true = We don't care about calls that occur with in the code range we are monitoring. 
* "start_log_on_exec" - Turns on logging once this instruction is called, no logging will occur until it is hit.
* "stop_log_on_exec" - Shuts off logging once this instruction is called.
* "code_start" - The offset of where the code begins. Do not specify the base address as this will be added to the offset automatically when the module loads.
* "code_end" - The offset of where the code ends. Same as above.
* "code_ignore" - An array of offsets of an instruction we want to ignore helpful for when a certain instruction is called excessively.
* "code_add" - Arbitrary addresses to have calls added to logging.
* "data_start" - The offset of where the data writes you care about start. 
* "data_end" - The offset of where the data writes you care about end.
* "data_ignore" - An array of offsets of addresses where writes occur that you don't want logged."
* "data_add" - Arbitrary addresses to have write calls added to logging.

### Example configuration file
```javascript
{
  "image_name": "nc.exe",
	"outfile": "C:\\source\\pin-latest\\ia32\\bin\\obfusc.log",
	"hook_functions": true,
	"disable_log_calls": false,
	"disable_log_writes": false,
	"disable_log_reads": true,
	"ignore_internal_calls": true,
	"code_start": 0x2B08C,
	"code_end": 0x43E78,
	"code_ignore": [0x2e333,0x2fde7,0x2f1cc,0x2ed19],
	"code_add": [0x2b000],
	"data_start": 0x2022C,
	"data_end": 0x43E78,
	"data_ignore": [0x2e330,0x3197a,0x2e068]
	"data_add": [0x20218]
}
```
### Running the tool
C:\pin> pin -t Ninko.dll -c ninko.config -- D:\tools\nc.exe

### Sample Output
```
Validating config...
Loading D:\tools\nc.exe
Start 0x00400000-0x0040ffff
Monitoring calls from code 0x400000-0x403f14
Monitoring data writes from 0x40e000-0x40fdff
Loading C:\Windows\SYSTEM32\KERNEL32.DLL
Start 0x75ba0000-0x75ccffff
RTN: GetProcAddress
Loading C:\Windows\SYSTEM32\KERNELBASE.dll
Start 0x75f40000-0x75fe5fff
RTN: GetProcAddress
Loading C:\Windows\SYSTEM32\ntdll.dll
Start 0x77560000-0x776b6fff
Loading C:\Windows\SYSTEM32\WS2_32.dll
Start 0x74df0000-0x74e3ffff
Loading C:\Windows\SYSTEM32\RPCRT4.dll
Start 0x75880000-0x7592bfff
Loading C:\Windows\SYSTEM32\NSI.dll
Start 0x75930000-0x75937fff
Loading C:\Windows\SYSTEM32\SspiCli.dll
Start 0x74ba0000-0x74bbbfff
Loading C:\Windows\SYSTEM32\CRYPTBASE.dll
Start 0x74b90000-0x74b98fff
Loading C:\Windows\SYSTEM32\sechost.dll
Start 0x75680000-0x756b3fff
Loading C:\Windows\SYSTEM32\bcryptPrimitives.dll
Start 0x74b30000-0x74b80fff
CL [tid:0] eip:0x4033f7 e8 c2 0b 00 00          call 0x403fbe ; target:0x403fbe name:.text
CL [tid:0] eip:0x40340b 74 05                   jz 0x403412 ; target:0x403412 name:.text
CL [tid:0] eip:0x403414 e8 0c 0d 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x403438 a3 80 e7 40 00          mov dword ptr [0x40e780], eax ; [ea:0x40e780] = 0x02040e20 [size:4]
CL [tid:0] eip:0x40343d e8 e3 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x403464 a3 84 e7 40 00          mov dword ptr [0x40e784], eax ; [ea:0x40e784] = 0x02040e40 [size:4]
CL [tid:0] eip:0x403469 e8 b7 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x40348c 89 15 ec e9 40 00       mov dword ptr [0x40e9ec], edx ; [ea:0x40e9ec] = 0x02042180 [size:4]
CL [tid:0] eip:0x403492 e8 8e 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x4034b4 89 15 40 ea 40 00       mov dword ptr [0x40ea40], edx ; [ea:0x40ea40] = 0x02044190 [size:4]
CL [tid:0] eip:0x4034ba e8 66 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x4034dc 89 15 44 ea 40 00       mov dword ptr [0x40ea44], edx ; [ea:0x40ea44] = 0x02040e60 [size:4]
CL [tid:0] eip:0x4034e2 e8 3e 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x403501 89 15 e8 e9 40 00       mov dword ptr [0x40e9e8], edx ; [ea:0x40e9e8] = 0x02040f70 [size:4]
CL [tid:0] eip:0x403507 e8 19 0c 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x403527 89 15 90 e7 40 00       mov dword ptr [0x40e790], edx ; [ea:0x40e790] = 0x02041080 [size:4]
CL [tid:0] eip:0x40352d e8 f3 0b 00 00          call 0x404125 ; target:0x404125 name:.text
WR [tid:0] eip:0x403535 a3 48 ea 40 00          mov dword ptr [0x40ea48], eax ; [ea:0x40ea48] = 0x020410d8 [size:4]
WR [tid:0] eip:0x40353d 89 1d cc e7 40 00       mov dword ptr [0x40e7cc], ebx ; [ea:0x40e7cc] = 0x00000000 [size:4]
WR [tid:0] eip:0x403543 c7 05 60 e0 40 00 04 00 00 00 mov dword ptr [0x40e060], 0x4 ; [ea:0x40e060] = 0x00000004 [size:4]
CL [tid:0] eip:0x40356c e8 b4 0b 00 00          call 0x404125 ; target:0x404125 name:.text
CL [tid:0] eip:0x403598 e8 88 0b 00 00          call 0x404125 ; target:0x404125 name:.text
CL [tid:0] eip:0x4035c2 e8 f1 0b 00 00          call 0x4041b8 ; target:0x4041b8 name:.text
CL [tid:0] eip:0x4035cc e8 75 0d 00 00          call 0x404346 ; target:0x404346 name:.text
CL [tid:0] eip:0x4035d8 e8 73 5c 00 00          call 0x409250 ; target:0x409250 name:unnamedImageEntryPoint
WR [tid:0] eip:0x4035e2 a3 6c e7 40 00          mov dword ptr [0x40e76c], eax ; [ea:0x40e76c] = 0x00000008 [size:4]
CL [tid:0] eip:0x4035f2 76 04                   jbe 0x4035f8 ; target:0x4035f8 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x40360d 7f f1                   jnle 0x403600 ; target:0x403600 name:.text
CL [tid:0] eip:0x403603 0f 84 73 00 00 00       jz 0x40367c ; target:0x40367c name:.text
CL [tid:0] eip:0x40367f eb 90                   jmp 0x403611 ; target:0x403611 name:.text
WR [tid:0] eip:0x403617 a3 6c e7 40 00          mov dword ptr [0x40e76c], eax ; [ea:0x40e76c] = 0x00000000 [size:4]
CL [tid:0] eip:0x40361e 74 18                   jz 0x403638 ; target:0x403638 name:.text
CL [tid:0] eip:0x40363e e8 9d 13 00 00          call 0x4049e0 ; target:0x4049e0 name:.text
CL [tid:0] eip:0x403653 e8 88 13 00 00          call 0x4049e0 ; target:0x4049e0 name:.text
CL [tid:0] eip:0x40365d 74 03                   jz 0x403662 ; target:0x403662 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x403693 75 dd                   jnz 0x403672 ; target:0x403672 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x403693 75 dd                   jnz 0x403672 ; target:0x403672 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x403693 75 dd                   jnz 0x403672 ; target:0x403672 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x403693 75 dd                   jnz 0x403672 ; target:0x403672 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x403693 75 dd                   jnz 0x403672 ; target:0x403672 name:.text
CL [tid:0] eip:0x403675 75 0a                   jnz 0x403681 ; target:0x403681 name:.text
CL [tid:0] eip:0x403686 75 05                   jnz 0x40368d ; target:0x40368d name:.text
CL [tid:0] eip:0x4036ac e8 bf e6 ff ff          call 0x401d70 ; target:0x401d70 name:.text
CL [tid:0] eip:0x401d85 e8 c6 f9 ff ff          call 0x401750 ; target:0x401750 name:.text
WR [tid:0] eip:0x40175f c7 05 54 e7 40 00 00 00 00 00 mov dword ptr [0x40e754], 0x0 ; [ea:0x40e754] = 0x00000000 [size:4]
CL [tid:0] eip:0x40176f e8 6c ff ff ff          call 0x4016e0 ; target:0x4016e0 name:.text
WR [tid:0] eip:0x4016ed a3 58 e7 40 00          mov dword ptr [0x40e758], eax ; [ea:0x40e758] = 0x00000001 [size:4]
WR [tid:0] eip:0x4016f2 a3 40 e7 40 00          mov dword ptr [0x40e740], eax ; [ea:0x40e740] = 0x00000001 [size:4]
WR [tid:0] eip:0x4016f7 a3 48 e7 40 00          mov dword ptr [0x40e748], eax ; [ea:0x40e748] = 0x00000001 [size:4]
WR [tid:0] eip:0x4016fc c7 05 50 e7 40 00 00 00 00 00 mov dword ptr [0x40e750], 0x0 ; [ea:0x40e750] = 0x00000000 [size:4]
CL [tid:0] eip:0x401706 e8 2c 2a 00 00          call 0x404137 ; target:0x404137 name:.text
WR [tid:0] eip:0x401713 a3 44 e7 40 00          mov dword ptr [0x40e744], eax ; [ea:0x40e744] = 0x00000000 [size:4]
CL [tid:0] eip:0x401718 75 0f                   jnz 0x401729 ; target:0x401729 name:.text
CL [tid:0] eip:0x40172c 75 0f                   jnz 0x40173d ; target:0x40173d name:.text
WR [tid:0] eip:0x401747 89 0d 4c e7 40 00       mov dword ptr [0x40e74c], ecx ; [ea:0x40e74c] = 0x00000001 [size:4]
CL [tid:0] eip:0x40174d c3                      ret  ; target:0x401774 name:.text
CL [tid:0] eip:0x40178e 74 09                   jz 0x401799 ; target:0x401799 name:.text
CL [tid:0] eip:0x4017ad 74 15                   jz 0x4017c4 ; target:0x4017c4 name:.text
CL [tid:0] eip:0x4017c6 74 06                   jz 0x4017ce ; target:0x4017ce name:.text
CL [tid:0] eip:0x4017d8 75 07                   jnz 0x4017e1 ; target:0x4017e1 name:.text
WR [tid:0] eip:0x4017e4 89 2d 58 e7 40 00       mov dword ptr [0x40e758], ebp ; [ea:0x40e758] = 0x00000002 [size:4]
WR [tid:0] eip:0x4017ee 89 15 40 e7 40 00       mov dword ptr [0x40e740], edx ; [ea:0x40e740] = 0x00000002 [size:4]
CL [tid:0] eip:0x4017f4 eb 06                   jmp 0x4017fc ; target:0x4017fc name:.text
CL [tid:0] eip:0x4017fe 74 4e                   jz 0x40184e ; target:0x40184e name:.text
WR [tid:0] eip:0x401857 a3 58 e7 40 00          mov dword ptr [0x40e758], eax ; [ea:0x40e758] = 0x00000001 [size:4]
CL [tid:0] eip:0x401866 c3                      ret  ; target:0x401d8a name:.text
CL [tid:0] eip:0x401d8d c3                      ret  ; target:0x4036b1 name:.text
CL [tid:0] eip:0x4036b9 0f 84 6a 02 00 00       jz 0x403929 ; target:0x403929 name:.text
CL [tid:0] eip:0x403930 74 32                   jz 0x403964 ; target:0x403964 name:.text
CL [tid:0] eip:0x40396a 74 18                   jz 0x403984 ; target:0x403984 name:.text
CL [tid:0] eip:0x40398b 74 4a                   jz 0x4039d7 ; target:0x4039d7 name:.text
CL [tid:0] eip:0x4039f4 e8 a7 e7 ff ff          call 0x4021a0 ; target:0x4021a0 name:.text
WR [tid:0] eip:0x4021a8 c7 05 cc e7 40 00 00 00 00 00 mov dword ptr [0x40e7cc], 0x0 ; [ea:0x40e7cc] = 0x00000000 [size:4]
CL [tid:0] eip:0x4021ba e8 66 1f 00 00          call 0x404125 ; target:0x404125 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021e9 75 f6                   jnz 0x4021e1 ; target:0x4021e1 name:.text
CL [tid:0] eip:0x4021ed e8 ea 1d 00 00          call 0x403fdc ; target:0x403fdc name:.text
CL [tid:0] eip:0x402206 74 23                   jz 0x40222b ; target:0x40222b name:.text
CL [tid:0] eip:0x40222b e8 a6 1d 00 00          call 0x403fd6 ; target:0x403fd6 name:.text
Loading C:\Windows\system32\napinsp.dll
Start 0x71b10000-0x71b1ffff
Loading C:\Windows\SYSTEM32\msvcrt.dll
Start 0x756c0000-0x75770fff
71b11c76 = GetProcAddress(C:\Windows\system32\napinsp.dll,NSPStartup) [caller:74dfffe0]
```
