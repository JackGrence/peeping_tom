#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#define FAKE_STDOUT "/tmp/fake_stdout"
#define FAKE_STDERR "/tmp/fake_stderr"

struct fake_fds
{
    int stdout_for_change;
    int stdout_for_monitor;
    int stderr_for_change;
    int stderr_for_monitor;
};


int myread(int fd, char *buf, size_t count);
void *getoutput(void *arg_fds);
void change_fd(pid_t victim_pid);

int finish = 1;

int main(int argc, const char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s <pid>\n", argv[0]);
        return 0;
    }
    int fake_stdout_for_change, fake_stdout_for_monitor;
    int fake_stderr_for_change, fake_stderr_for_monitor;
    char buf[2];
    unlink(FAKE_STDERR);
    unlink(FAKE_STDOUT);
    fake_stdout_for_change = open(FAKE_STDOUT, O_RDWR | O_CREAT);
    fake_stdout_for_monitor = open(FAKE_STDOUT, O_RDWR | O_CREAT);
    fake_stderr_for_change = open(FAKE_STDERR, O_RDWR | O_CREAT);
    fake_stderr_for_monitor = open(FAKE_STDERR, O_RDWR | O_CREAT);

    printf("PID: %d\nfake stdout's fd: %d\nfake stderr's fd: %d\n",
            getpid(), fake_stdout_for_change, fake_stderr_for_change);

    pid_t victim_pid = atoi(argv[1]);

    printf("change fd\n");
    change_fd(victim_pid);

    pthread_t monitor;
    struct fake_fds fds = {
        .stdout_for_change = fake_stdout_for_change,
        .stdout_for_monitor = fake_stdout_for_monitor,
        .stderr_for_change = fake_stderr_for_change,
        .stderr_for_monitor = fake_stderr_for_monitor,
    };

    if(pthread_create(&monitor, NULL, getoutput, &fds))
    {
        fprintf(stderr, "fail create pthread\n");
    }

    do
    {
        printf("\nenter 0 to exit:\n");
        fflush(stdout);
        read(0, buf, 2);
        if(buf[0] == '0')
            finish = 0;
    }
    while(finish);

    if(pthread_join(monitor, NULL))
    {
        fprintf(stderr, "fail join pthread\n");
    }

    printf("chfd again\n");
    fflush(stdout);
    change_fd(victim_pid);

    close(fake_stdout_for_change);
    close(fake_stdout_for_monitor);
    close(fake_stderr_for_change);
    close(fake_stderr_for_monitor);
    return 0;
}

void *getoutput(void *arg_fds)
{
    int cnt, cnt2;
    char buf[2];
    struct fake_fds *fds = (struct fake_fds *) arg_fds;
    do
    {
        cnt2 = myread(fds->stderr_for_monitor, buf, 1);
        if(cnt2 > 0)
        {
            do
            {
                write(fds->stderr_for_change, buf, cnt2);
                if(buf[0] == '\n')
                    break;
            }
            while((cnt2 = myread(fds->stderr_for_monitor, buf, 1)));
        }

        cnt = myread(fds->stdout_for_monitor, buf, 1);
        if(cnt > 0)
        {
            do
            {
                write(fds->stdout_for_change, buf, cnt);
            }
            while((cnt = myread(fds->stdout_for_monitor, buf, 1)));
        }

        if(cnt <= 0 && cnt2 <= 0)
        {
            usleep(100);
        }
    }
    while(finish);
}

int myread(int fd, char *buf, size_t count)
{
    int cnt;
    cnt = read(fd, buf, count);
    if(cnt < 0)
    {
        printf("read fail\n");
        return -1;
    }
    else
    {
        printf("%.*s", cnt, buf);
        fflush(stdout);
        return cnt;
    }
}

void change_fd(pid_t victim_pid)
{
    char cmd[256];
    sprintf(cmd, "./chfd %d 3 %d 1",
            getpid(), victim_pid);
    system(cmd);

    sprintf(cmd, "./chfd %d 5 %d 2",
            getpid(), victim_pid);
    system(cmd);
}
