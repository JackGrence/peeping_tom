CFLAG=-lpthread

echo_fd: echo_fd.c
	gcc $< -o $@ ${CFLAG}
