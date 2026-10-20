#include "LocalServer.hpp"
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <fcntl.h>
#include <poll.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>

mygame::LocalServer::~LocalServer() { stop(); }

bool mygame::LocalServer::start(const Options &options, uint16_t &port)
{
    if (mPid > 0) return false;
    int pipeFds[2];
    if (pipe2(pipeFds, O_CLOEXEC) != 0) return false;
    pid_t parent = getpid();
    mPid = fork(); // Called before Engine/SDL starts any worker threads.
    if (mPid == 0)
    {
        close(pipeFds[0]);
        if (prctl(PR_SET_PDEATHSIG, SIGTERM) != 0 || getppid() != parent || fcntl(pipeFds[1], F_SETFD, 0) < 0)
            _exit(1);
        char fd[16], portText[16];
        std::snprintf(fd, sizeof(fd), "%d", pipeFds[1]);
        std::snprintf(portText, sizeof(portText), "%u", options.port);
        const char *args[16]{options.serverExecutable, "--ready-fd", fd};
        unsigned count = 3;
        if (options.explicitPort) { args[count++] = "--port"; args[count++] = portText; }
        if (*options.level)
        {
            args[count++] = "--level"; args[count++] = options.level;
            args[count++] = "--level-root"; args[count++] = options.root;
        }
        if (options.explicitConfig) { args[count++] = "--config"; args[count++] = options.config; }
        execv(options.serverExecutable, const_cast<char *const *>(args));
        std::perror("Local server exec");
        _exit(1);
    }
    close(pipeFds[1]);
    if (mPid < 0) { close(pipeFds[0]); return false; }
    pollfd ready{pipeFds[0], POLLIN, 0};
    int result;
    do { result = poll(&ready, 1, 10000); } while (result < 0 && errno == EINTR);
    bool success = result > 0 && (ready.revents & POLLIN) && read(pipeFds[0], &port, sizeof(port)) == sizeof(port) && port;
    close(pipeFds[0]);
    if (!success) stop();
    return success;
}

bool mygame::LocalServer::alive()
{
    if (mPid <= 0) return false;
    int status = 0;
    pid_t result = waitpid(mPid, &status, WNOHANG);
    if (result == mPid || (result < 0 && errno == ECHILD)) { mPid = -1; return false; }
    return result == 0 || (result < 0 && errno == EINTR);
}

void mygame::LocalServer::reload()
{
    if (alive()) kill(mPid, SIGHUP);
}

void mygame::LocalServer::stop()
{
    if (mPid <= 0) return;
    if (alive()) kill(mPid, SIGTERM);
    for (unsigned i = 0; i < 200 && alive(); ++i) usleep(10000);
    if (mPid > 0)
    {
        kill(mPid, SIGKILL);
        while (waitpid(mPid, nullptr, 0) < 0 && errno == EINTR) {}
        mPid = -1;
    }
}
