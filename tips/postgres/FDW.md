# Signal Handling inside FDW code - Leader Participation

* From ChatGPT


**prompt:** `I'm working on a code for a Foreign Data Wrapper (FDW) for Postgres. There's a case when postgres is processing a parallel query with the option leader_participation=on where on of the workers receives a signal and as a consequence my recv() on a socket get interrupted. Do you know what and why this signal is sent? And by whom? 
`


**Response:**

When Postgres processes parallel queries, various signals are used to manage the coordination and lifecycle of parallel workers. The signal that is causing your recv() to get interrupted is likely **SIGUSR1**. Here’s a detailed explanation of why this happens, who sends the signal, and what it implies:

Signals in PostgreSQL Parallel Queries
Parallel Query Mechanism:

PostgreSQL uses parallel workers to speed up query processing. These workers are forked by the leader process and can execute parts of the query plan concurrently.
The leader process manages these workers, and workers need to communicate and synchronize with the leader and possibly with each other.
Signal Handling:

**SIGUSR1** is a user-defined signal that PostgreSQL uses for inter-process communication. This signal is often used to wake up processes waiting for certain events, such as new work, messages, or termination requests.
**SIGUSR1** can be sent by the leader process to one or more parallel workers to notify them of various events, such as query cancellation, new messages to process, or shutdown requests.
Leader Participation:

When leader_participation is set to on, the leader process itself participates in executing the parallel plan. This means the leader can also perform tasks typically assigned to workers, and workers might need to signal the leader as well.
Interruption of recv():

If a parallel worker receives **SIGUSR1** while it is blocked on a recv() call (or any blocking system call), the system call will be interrupted and return **EINTR**.
This interruption is a normal part of signal handling in Unix-like systems. The process needs to handle the signal and typically retry the recv() call or handle the event that caused the signal.
Who Sends the Signal and Why?
Who Sends the Signal:

The signal can be sent by the leader process, other workers, or the postmaster (the parent process managing the PostgreSQL instance).
Why the Signal is Sent:

Query Cancellation: If the client cancels the query, the leader process sends **SIGUSR1** to all workers to notify them of the cancellation.
New Messages: To notify a worker that there are new messages in a shared queue or that there is new work to be done.
Shutdown Requests: To gracefully shut down workers if the query execution is completed or if the backend process is terminating.
Timeouts or Interruptions: For handling query timeouts or other asynchronous events.
Handling Interrupted recv() in FDW Code
When you receive **EINTR** in your FDW code during a recv() call, you should:

Retry the System Call:

Loop and retry the recv() call unless another condition requires you to break out of the loop.
Check for Query Cancellation:

If **EINTR** is received, check if the query has been cancelled. PostgreSQL provides macros like CHECK_FOR_INTERRUPTS() to handle query cancellations.

**Example Code Snippet:**
```c
Here’s an example of handling **EINTR** in a loop:
#include <errno.h>
#include <signal.h>

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t n;
    while (1)
    {
        n = recv(sockfd, buf, len, flags);
        if (n < 0)
        {
            if (errno == EINTR)
            {
                // Interrupted by a signal, retry the recv call
                CHECK_FOR_INTERRUPTS();
                continue;
            }
            else
            {
                // Handle other errors
                perror("recv");
                break;
            }
        }
        break;
    }
    return n;
}
```

In this example, CHECK_FOR_INTERRUPTS() is used to handle any query cancellation requests if the signal causing **EINTR** was due to a query cancellation.

Conclusion
The interruption of recv() by a signal such as **SIGUSR1** is a normal part of signal handling in PostgreSQL's parallel query execution. Properly handling **EINTR** by retrying the recv() call and checking for query cancellations ensures robust and correct behavior in your FDW code.
