# rawdump
I wrote rawdump because I am sick of available hex dump utilities that always
assume that printf is used. The fact is, a lot of developers can only use
debug routines on the platform.

To make it better, I also included the address (truncated to 32-bit), and
ASCII translation.

## Usage
Just include the code portion (excluding the #include and main()) into your
source code. For simplicity, I made API static and did not include .h file.

Use of rawdump is made up of 3 lines
```
char *s;
while (s = rawdump(data, data_size))
    DEBUG("%s\n", s);
```
(You can also find RAWPRINT macro which do it in 1 line)

## Sample Output
```
DBG: C0BA1F70:68336C6C 302C2077 6F726C64 2E205468 h3ll0, world. Th
DBG: C0BA1F80:69732069 73206120 74657374 2E204974 is is a test. It
DBG: C0BA1F90:2073686F 756C6420 6265206F 76657220  should be over
DBG: C0BA1FA0:32206C69 6E6573                     2 lines
```

The data starts from 2nd column. It is the address of data (truncated to
32 bits), followed by data in 32-bit words. After 16 bytes, it prints
the ASCII translation of the binary data.

## Details
First thing you will notice is that rawdump does not print. Instead, it stores
the output in a string so you can print it out in your own way.

Furthermore, the routine requires you to call it repeatedly, populating 1 line
after each call. This allows you to break out if needed.

The API makes use of static variables to keep track of its states.
As a result, it is not thread safe. But I think it is a good compromise for
ease-of-use. Calling the same API twice using the same data pointer will
continue the dump until you have reached data_size. If you call it again, it
will start from the beginning. If you switch to a different pointer, it will
also reset. The API can be modified to thread safe if there is enough demand
for it.

## Limitations
As mentioned before, the API is not thread safe.

The address that it prints starts from the data pointer. It may not start
from a 16 bytes boundary. In fact, it may not even start from a word
boundary, so it can be confusing sometimes.
