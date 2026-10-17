#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void usage(const char *p) {
    fprintf(stderr, "Usage: %s [--log] \"command 1\" \"command 2\" [\"command 3\" ...]\n", p);
}
static void close_pipes(int (*pipes)[2], size_t n) {
    if (!pipes) return;
    for (size_t i=0; i<n; ++i) {
        if (pipes[i][0] >= 0) { close(pipes[i][0]); pipes[i][0] = -1; }
        if (pipes[i][1] >= 0) { close(pipes[i][1]); pipes[i][1] = -1; }
    }
}
static void log_event(size_t i, const char *event, pid_t pid, const char *cmd) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        fprintf(stderr, "[%lld.%03ld] stage=%zu pid=%ld %s: %s\n",
                (long long)ts.tv_sec, ts.tv_nsec/1000000L, i+1, (long)pid, event, cmd);
    else
        fprintf(stderr, "stage=%zu pid=%ld %s: %s\n", i+1, (long)pid, event, cmd);
    fflush(stderr);
}
int main(int argc, char **argv) {
    int logs=0, first=1;
    if (argc>1 && strcmp(argv[1],"--log")==0) { logs=1; first=2; }
    if (argc-first<2) { usage(argv[0]); return EXIT_FAILURE; }

    size_t count=(size_t)(argc-first), npipe=count-1;
    int (*pipes)[2]=calloc(npipe,sizeof(*pipes));
    pid_t *pids=calloc(count,sizeof(*pids));
    int *statuses=calloc(count,sizeof(*statuses));
    if (!pipes || !pids || !statuses) {
        perror("calloc"); free(pipes); free(pids); free(statuses); return EXIT_FAILURE;
    }
    for (size_t i=0;i<npipe;i++) pipes[i][0]=pipes[i][1]=-1;
    size_t made=0;
    for (;made<npipe;made++) {
        if (pipe(pipes[made])==-1) {
            perror("pipe"); close_pipes(pipes,made);
            free(pipes); free(pids); free(statuses); return EXIT_FAILURE;
        }
    }

    size_t started=0; int launch_failed=0;
    for (size_t i=0;i<count;i++) {
        pid_t pid=fork();
        if (pid==-1) { perror("fork"); launch_failed=1; break; }
        if (pid==0) {
            if (i>0 && dup2(pipes[i-1][0],STDIN_FILENO)==-1) { perror("dup2 stdin"); _exit(126); }
            if (i+1<count && dup2(pipes[i][1],STDOUT_FILENO)==-1) { perror("dup2 stdout"); _exit(126); }
            close_pipes(pipes,npipe);
            if (logs) log_event(i,"START",getpid(),argv[first+(int)i]);
            execl("/bin/sh","sh","-c",argv[first+(int)i],(char *)NULL);
            perror("execl"); _exit(127);
        }
        pids[started++]=pid;
        if (logs) log_event(i,"LAUNCHED",pid,argv[first+(int)i]);
    }
    close_pipes(pipes,npipe);

    int result=launch_failed ? EXIT_FAILURE : EXIT_SUCCESS;
    for (size_t i=0;i<started;i++) {
        pid_t w;
        do { w=waitpid(pids[i],&statuses[i],0); } while (w==-1 && errno==EINTR);
        if (w==-1) { perror("waitpid"); result=EXIT_FAILURE; continue; }
        if (WIFEXITED(statuses[i])) {
            int code=WEXITSTATUS(statuses[i]);
            if (logs) { char e[64]; snprintf(e,sizeof e,"EXIT status=%d",code); log_event(i,e,pids[i],argv[first+(int)i]); }
            if (code!=0) result=EXIT_FAILURE;
        } else if (WIFSIGNALED(statuses[i])) {
            if (logs) { char e[64]; snprintf(e,sizeof e,"SIGNALED signal=%d",WTERMSIG(statuses[i])); log_event(i,e,pids[i],argv[first+(int)i]); }
            result=EXIT_FAILURE;
        } else result=EXIT_FAILURE;
    }
    free(pipes); free(pids); free(statuses);
    return result;
}
