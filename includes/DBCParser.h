#define MAX_LINE_LENGTH 256
#define MAX_SIGNALS 64
#define MAX_MESSAGES 256
#define MAX_SIGNAL_NAME_LENGTH 64
#define MAX_MESSAGE_NAME_LENGTH 64
#define MAX_NODE_NAME_LENGTH 64
#define MAX_UNIT_NAME_LENGTH 64

typedef struct {
    char name[MAX_SIGNAL_NAME_LENGTH];
    int start_bit;
    int length;
    int endian;
    char signd;
    float scale;
    float offset;
    float min;
    float max;
    char unit[MAX_UNIT_NAME_LENGTH];
    char reciever[MAX_NODE_NAME_LENGTH];
} Signal;

typedef struct {
    int id;
    char name[MAX_MESSAGE_NAME_LENGTH];
    int dlc; // Data Length Code
    char sender[MAX_NODE_NAME_LENGTH];
    Signal signals[MAX_SIGNALS];
    int signal_count;
} Message;

typedef struct {
    Message messages[MAX_MESSAGES];
    int message_count;
} DBC;

int parse_dbc_line(char *line, DBC *dbc);

int parse_dbc_file(const char *filename, DBC *dbc);

void print_dbc(const DBC *dbc);