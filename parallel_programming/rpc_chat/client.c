#include <ncursesw/ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chat.h"

#define MSG_LEN 32
#define DEFAULT_HOST "localhost"

typedef struct
{
    CLIENT *client;
    WINDOW *output;
} read_args_t;

int connect_user(CLIENT **, char *, user_info **);
int disconnect_user(CLIENT **, user_info **, int);
void logged_users_num(CLIENT *);
void send_message(int, CLIENT *, msg);
void *read_messages(void *);
char *status_string(STATUS);
int menu();
int is_command(char *input);
void print_help(WINDOW *output);
void print_welcome_message(WINDOW *output);

int main(int argc, char *argv[])
{
    CLIENT *client = NULL;
    user_info *user = NULL;
    int is_com = 0;
    int userid;

    userid = connect_user(&client, DEFAULT_HOST, &user);

    int done = 0;
    WINDOW *input, *output;
    char buffer[1024];

    initscr();
    cbreak();
    echo();
    input = newwin(1, COLS, LINES - 1, 0);
    output = newwin(LINES - 1, COLS, 0, 0);

    read_args_t args = {
        .client = client,
        .output = output};
    pthread_t read_thread;
    pthread_create(&read_thread, NULL, read_messages, (void *)(&args));
    wmove(output, LINES - 2, 0);
    scrollok(output, TRUE);
    print_welcome_message(output);

    while (!done)
    {
        mvwprintw(input, 0, 0, "> ");
        if (wgetnstr(input, buffer, COLS - 4) != OK)
        {
            break;
        }
        werase(input);
        wrefresh(output);

        if (is_command(buffer))
        {
            if (!strncmp(buffer, "/h", 2))
            {
                print_help(output);
            }
        }
        else
        {
            msg message;
            sscanf(buffer, "\n%[^\n]%*c", message.text);
            send_message(userid, client, message);
        }
        done = ((*buffer == 4) || !(strncmp(buffer, "/q", 2))) ? 1 : 0; /* quit on control-D */
    }
    endwin();
    pthread_join(read_thread, NULL);
    pthread_cancel(read_thread);    
    disconnect_user(&client, &user, userid);
    return 0;
}

int connect_user(CLIENT **client, char *host, user_info **user)
{
    *user = malloc(sizeof(user_info));
    *client = clnt_create(host, RPC_CHAT, RPC_CHAT_VERSION, "udp");

    printf("Login: ");
    scanf("%9s", (*user)->login);
    return *connect_1(*user, *client);
}

int disconnect_user(CLIENT **client, user_info **user, int userid)
{
    int result = 0;
    result = *disconnect_1(&userid, *client);

    if (result != 0)
        printf("disconnect_user error\n");

    free(*client);
    free(*user);

    return result;
}

void send_message(int userid, CLIENT *client, msg message)
{
    message.userid = userid;

    if (*send_1(&message, client) < 0)
        printf("Error sending message\n");
}

void *read_messages(void *read_arg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    read_args_t arg = *(read_args_t *)(read_arg);
    int is_new_msg = 0;
    msg *message;

    user_info *user;
    int messages_num_cur = 0;
    int messages_num_prev = 0;
    void *p;
    char out_msg[MSG_LEN * 5];
    while (1)
    {
        messages_num_cur = *get_msg_num_1(p, arg.client);
        if (messages_num_cur > messages_num_prev)
        {
            for (int i = messages_num_prev; i < messages_num_cur; i++)
            {
                message = get_msg_1(&i, arg.client);
                user = get_user_info_1(&(message->userid), arg.client);
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                char date[128];
                sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                snprintf(out_msg, MSG_LEN * 5, "[%s] %s: %s", date, user->login, message->text);
                waddch(arg.output, '\n');
                waddstr(arg.output, out_msg);
                wrefresh(arg.output);
            }
            messages_num_prev = messages_num_cur;
        }
    }
}

void logged_users_num(CLIENT *client)
{
    void *p;
    printf("\n\n %d logged users\n", *get_user_info_num_1(p, client));
}

char *status_string(STATUS status)
{
    if (status == ONLINE)
        return "ONLINE";
    else
        return "OFFLINE";
}

int menu()
{
    int option;

    printf("\nChoose an option:\n");
    printf("\t0 - disconnect_user\n");
    printf("\t1 - Number of logged users\n");
    printf("\t2 - Send message\n");
    printf("\t3 - Read messages\n");
    printf("Option: ");

    scanf("%d", &option);
    return option;
}

int is_command(char *input)
{
    return (strlen(input) > 1 && input[0] == '/') ? 1 : 0;
}

void print_help(WINDOW *output)
{
    waddstr(output, "\n\n=================================\n\n");
    waddstr(output, "\tHELP MESSAGE\n");
    waddstr(output, "Available commands\n");
    waddstr(output, "\th show this help message\n");
    waddstr(output, "\tq exit chat\n");
    waddstr(output, "\n=================================\n\n");
    wrefresh(output);
}

void print_welcome_message(WINDOW *output)
{
    waddstr(output, "+-----------------------------------------+\n");
    waddstr(output, "|            WELCOME TO THE CHAT          |\n");
    waddstr(output, "|                                         |\n");
    waddstr(output, "| Messages starting with '/' are commands |\n");
    waddstr(output, "| # to show help-message enter '/h'       |\n");
    waddstr(output, "| # to exit chat enter '/q'               |\n");
    waddstr(output, "|                                         |\n");
    waddstr(output, "|          GOOD LUCK AND HAVE FUN         |\n");
    waddstr(output, "+-----------------------------------------+\n");
    wrefresh(output);
}