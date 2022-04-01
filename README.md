# LKDQQAQKCG-_Simple_Message_System
Simple Message Processor

## Purpose

* Become familiar with sempaphores
* Use multiple concurrent processes to write to a single file
* Create an environment such that two processes cannot simulatenously write to the file

## Task
* Geenrate 20 processes using master programm called `Master`
* Each process writes to a file called `cstest`
* All processes will use the same working directory
* Each child process will be executable and be called `slave`
* Each process will generate a message in the format *HH:MM:SS Queue nn File modified by process number xx*
* `HH:MM:SS` = `currentSystemTime`
* `nn` = `process id`
* `xx`  = `slave process id`

## Master.c

### Requirements
* Runs `n` slave processes at a time *(n < 20)*
* Executes with the following command `master -t ss n`
    * `ss` maximum time in seconds process should live *default=100*

### Master Impelementation
1. Check for command line argument
2. Validate command line argument
    1. Respond with appropiate error if needed
3. Allocation shared memory
4. Initiliaze semaphore
5. Execute slave processes
6. Start timer for slave process expiration
7. Deallocate shared memory
8. Terminate  

## Application Program **(slave)**
### Requirements
* Write message to file
* Use master semaphore
* Print time stamped status to logfile `logfile.xx` where `xx` is the childs process number
* Provide time stamped status on process start and end
* Critical Section should sleep for a random number of seconds [1,5]
* The critical section should only be entered 5 times max

<code>

    for ( i = 0; i < 5; i++ )

    {     
        acquire the semaphore;
        sleep for random amount of time (between 0 and 5 seconds);
        critical_section(); // Write into the file
        sleep for random amount of time (between 0 and 5 seconds);
        release the semaphore
    }

</code>

## Project Implementation
* `master` will create a specified number of slave processes.
* `master` initiates slave processes and waits for each to finish
* `master` kills processes that time out
* `master` will initialize a sempaphore
* `master` handle logfile for `slave` process
* `ctest` will be opened by `slave` process after aquiring semaphore
* `master` should print a message when an interupt signal `^C` is received
* Executables should be called `master` and `slave`

## Points to Remember
* use `fork`, `exec`, `wait`, `exit` to manage processes
* use `semctl` to call semaphores.
* issue warning when more than 20 processes are called for and proceed with max

## Termination Criteria
* If all the slaves have finished master should:
    * deallocate memory
    * terminate
* Terminates after a certain amount of time specified in config.h
* ctrl-c
* output time of termination to file no matter the mean of termination


