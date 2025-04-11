#include <stdio.h>
#include <string.h>
#include <mysql.h>

MYSQL *conn;

struct borrower
{
    int ID;
    char name[50];
    double mobile_no;
    char book_name[50];
};
struct login
{
    char username[50];
    int password;
};

void connectToDatabase()
{
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "root", "a2409samsung", "library_system", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
    else
    {
        printf("connected to my MySQL database!\n");
    }
}

int adminMode()
{
    struct login admin;
    int attempts = 0;
    const int authentication = 3;

    while (attempts < authentication)
    {
        printf("Enter the username: ");
        scanf("%s", admin.username);
        printf("Password: ");
        scanf("%d", &admin.password);

        char query[200];
        sprintf(query, "select * from login where username='%s' and password='%d' and role='admin'", admin.username, admin.password);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            return 0;
        }

        MYSQL_RES *res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0)
        {
            printf("you have enter the admin mode\n");
        }
        else
        {
            printf("Invalid Admin credentials. Try again.\n", attempts + 1, authentication);
            attempts++;
        }

        if (res)
            mysql_free_result(res);
    }

    printf("Too many failed attempts.Access denied.\n");
    return 0;
}
int userMode()
{
    struct login user;
    int attempts = 0;
    const int authentication = 3;
    while (attempts < authentication)
    {
        printf("Enter the username: ");
        scanf("%s", user.username);
        printf("Password: ");
        scanf("%d", &user.password);

        char query[200];
        sprintf(query, "select * from login where username='%s' and password='%d' and role='user'", user.username, user.password);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
            return 0;
        }

        MYSQL_RES *res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) > 0)
        {
            printf("you have enter the user mode");
        }
        else
        {
            printf("Invalid user credentials. Try again.\n", attempts + 1, authentication);
            attempts++;
        }

        if (res)
            mysql_free_result(res);
    }

    printf("Too many failed attempts.Access denied.\n");
    return 0;
}
int main()
{
    // connect to MySQL first
    connectToDatabase();

    int choose;
    while (1)
    {
        printf("\n--- Library Management System ---\n");
        printf("1.Admin Mode\n");
        printf("2.User Mode\n");
        printf("Enter your choose: ");
        scanf("%d", &choose);
        switch (choose)
        {
        case 1:
            adminMode();
            break;
        case 2:
            userMode();
            break;
        default:
            printf("Invalide please enter 1 or 2\n");
            break;
        }
    }

    // close MySQL connection
    mysql_close(conn);
    return 0;
}
