enum STATUS {OFFLINE = 0, ONLINE};

struct user_info {
  char login[10];
  STATUS status;
};

struct msg {
  int userid;
  char text[100];
};

program RPC_CHAT {
  version RPC_CHAT_VERSION {
    int CONNECT(user_info u) = 1;
    int DISCONNECT(int userid) = 2;
    int GET_USER_INFO_NUM() = 3;
    user_info GET_USER_INFO(int userid) = 4;
    int SEND(msg m) = 5;
    int GET_MSG_NUM() = 6;
    msg GET_MSG(int msgid) = 7;
  } = 1;
} = 200000001;
