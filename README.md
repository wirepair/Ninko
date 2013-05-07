#Ninko 
Ninko is a PIN tool created to aid in instrumenting obfuscated code. It works by logging only calls and writes specified for an exact range. This helps in limiting the amount of information you need to wade through to understand how a binary may or may not be working.

#Usage
Ninko works off of JSON formatted configuration files. There are a few things you must know about your binary before you can begin.

##Ninko Configuration Fields
Note "calls" can mean any type, direct or indirect, jmp, jz, call, ret etc.

* "image_name" - The image name: One would hope you'd know the name of the executable/dll you wish to monitor.
* "outfile" - Where to save the results to. 
* "hook_functions" - Whether or not to hook GetProcAddress or any hooks you add to the source.
* "dont_log_calls" - true = Ignore logging calls, false = Log.
* "dont_log_writes" - true = Ignore logging writes, false = Log.
* "ignore_internal_calls" - true = We don't care about calls that occur with in the code range we are monitoring. 
* "code_start" - The offset of where the code begins. Do not specify the base address as this will be added to the offset automatically when the module loads.
* "code_end" - The offset of where the code ends. Same as above
* "code_ignore" - An array of offsets of an instruction we want to ignore helpful for when a certain instruction is called excessively.
* "data_start" - The offset of where the data writes you care about start. 
* "data_end" - The offset of where the data writes you care about end.
* "data_ignore" - An array of offsets of addresses where writes occur that you don't want logged."

### Example configuration file
```javascript
{
  "image_name": "nc.exe",
	"outfile": "C:\\source\\pin-latest\\ia32\\bin\\obfusc.log",
	"hook_functions": true,
	"dont_log_calls": false,
	"dont_log_writes": false,
	"ignore_internal_calls": true,
	"code_start": 0x2B08C,
	"code_end": 0x43E78,
	"code_ignore": [0x2e333,0x2fde7,0x2f1cc,0x2ed19],
	"data_start": 0x2022C,
	"data_end": 0x43E78,
	"data_ignore": [0x2e330,0x3197a,0x2e068]
}
```
###Running the tool
C:\pin> pin -t Ninko.dll -c ninko.config -- D:\tools\nc.exe

