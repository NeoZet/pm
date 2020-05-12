#include <stdio.h>
#include <string.h>
#include "chat.h"

#define MAXUSERS 100
#define MAXMSGS 1000

int user_exists(user_info *u);
int new_user(user_info *u);

user_info users[MAXUSERS];
msg messages[MAXMSGS];
int users_num = 0;
int msgs_num = 0;

int * connect_1_svc(user_info *u, struct svc_req *rqstp)
{
  static int result;

  if(users_num < MAXUSERS)
  {
    int userid = user_exists(u);
    if(userid < 0)
      userid = new_user(u);
    else
      users[userid].status = ONLINE;
    result = userid;
  }
  else
    result = -1;

  return &result;
}

int * disconnect_1_svc(int *userid, struct svc_req *rqstp)
{
  static int result;

  if(*userid<0 || *userid>=users_num)
    result = -1;
  else
  {
    users[*userid].status = OFFLINE;
    result = 0;
  }

  return &result;
}

int * get_user_info_num_1_svc(void *argp, struct svc_req *rqstp)
{
  static int result;

  int i;
  int total_connections = 0;
  for(i=0; i<users_num; i++)
    if(users[i].status == ONLINE)
      total_connections++;
  result = total_connections;

  return &result;
}

user_info * get_user_info_1_svc(int *userid, struct svc_req *rqstp)
{
  static user_info result;

  if(*userid>=0 && *userid<users_num)
    result = users[*userid];

  return &result;
}

int * send_1_svc(msg *m, struct svc_req *rqstp)
{
  static int result;

  if(msgs_num < MAXMSGS)
  {
    messages[msgs_num] = *m;
    result = msgs_num++;
  }
  else
    result = -1;

  return &result;
}

int * get_msg_num_1_svc(void *argp, struct svc_req *rqstp)
{
  static int result;

  result = msgs_num;

  return &result;
}

msg * get_msg_1_svc(int *msgid, struct svc_req *rqstp)
{
  static msg result;

  if(*msgid>=0 && *msgid<msgs_num)
    result = messages[*msgid];

  return &result;
}

int user_exists(user_info *u)
{
  int i;
  for(i=0; i<users_num; i++)
    if(strcmp(users[i].login, u->login) == 0)
      return i;
  return -1;
}

int new_user(user_info *u)
{
  u->status = ONLINE;
  users[users_num] = *u;
  return users_num++;
}
