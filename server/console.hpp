#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <cstring>


bool console_is_running = false;

class ServerConsole
{
    public:
        ServerConsole();
        void stop(void);
        bool is_running(void);
    private:
        pthread_t thread;
};


void * console_logic(void *)
{
    printf("Started server console...\n");

    while(console_is_running)
    {
        char c [256];
        fgets(c,sizeof(c), stdin);
        if(c[0] == 'q')
        {
            console_is_running = false;
        }
        else
        {
            printf("Invalid console command!\n");
        }
    }
    return 0;
}

ServerConsole::ServerConsole(void)
{
    //Intialized values
    console_is_running = true;

    thread = pthread_t();

    pthread_create(&(this->thread), NULL, console_logic, NULL);
    
}

void ServerConsole::stop(void)
{
    if(this->thread != 0)
    {
        pthread_join(this->thread, NULL);
    }
}

bool ServerConsole::is_running(void)
{
    return console_is_running;
}

#endif