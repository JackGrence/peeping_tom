# Peeping Tom

A simple tool depend on [fdhijack](https://github.com/JackGrence/fdhijack) to keep a watch on someone's process.

## Getting Started

```
git clone https://github.com/JackGrence/peeping_tom.git
```

### Prerequisites

* [fdhijack](https://github.com/JackGrence/fdhijack)

### Installing

Build echo_fd with:

```
make
```

### Usage

Make sure echo_fd, chfd, fdhj.ko and fdhj_load in current directory.

Load the kernel module fdhj.ko:

```
./fdhj_load
```

Monitor process with pid:

```
./echo_fd <pid>
```

### Example

In root's terminal:

```
root@ubuntu:~/chfd_test# ./fdhj_load
root@ubuntu:~/chfd_test# ps aux | grep bash     
root        959  0.0  0.6  23556  6320 pts/0    S    16:55   0:03 -bash
victim     1007  0.0  0.5  22472  5116 pts/1    Ss+  16:55   0:00 -bash
root       5338  0.0  0.0  11284   904 pts/0    S+   19:50   0:00 grep --color=auto bash
root@ubuntu:~/chfd_test# ./echo_fd 1007
PID: 5343
fake stdout's fd: 3
fake stderr's fd: 5
change fd

enter 0 to exit:

victim@ubuntu:~$ ll
total 32
drwxr-xr-x 3 victim victim 4096 Feb  6 17:58 ./
drwxr-xr-x 7 root   root   4096 Feb  5 18:35 ../
-rw------- 1 victim victim 2173 Feb  9 05:19 .bash_history
-rw-r--r-- 1 victim victim  220 Feb  5 18:35 .bash_logout
-rw-r--r-- 1 victim victim 3771 Feb  5 18:35 .bashrc
drwx------ 2 victim victim 4096 Feb  5 18:36 .cache/
-rw-r--r-- 1 victim victim  655 Feb  5 18:35 .profile
-rwxr-xr-x 1 root   root     67 Feb  6 17:58 echo.sh*
victim@ubuntu:~$ echo hihi
hihi
victim@ubuntu:~$ 

enter 0 to exit:
0
chfd again
root@ubuntu:~/chfd_test# 
```

In victim's terminal:

```
victim@ubuntu:~$ ll
total 32
drwxr-xr-x 3 victim victim 4096 Feb  6 17:58 ./
drwxr-xr-x 7 root   root   4096 Feb  5 18:35 ../
-rw------- 1 victim victim 2173 Feb  9 05:19 .bash_history
-rw-r--r-- 1 victim victim  220 Feb  5 18:35 .bash_logout
-rw-r--r-- 1 victim victim 3771 Feb  5 18:35 .bashrc
drwx------ 2 victim victim 4096 Feb  5 18:36 .cache/
-rw-r--r-- 1 victim victim  655 Feb  5 18:35 .profile
-rwxr-xr-x 1 root   root     67 Feb  6 17:58 echo.sh*
victim@ubuntu:~$ echo hihi
hihi
victim@ubuntu:~$ 
```

## Reference

* [File Descriptor Hijacking](http://www.phrack.org/issues/51/5.html#article)
