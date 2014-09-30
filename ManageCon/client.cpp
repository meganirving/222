#include <iostream>

using namespace std;

// TODO: replace with class & add inherited classes (in new file)
struct user
{
    int type;
};

int main()
{
    // welcome (ie: splashscreen)
    cout << "Welcome to ManageCon, the premier Conference Management System" << endl;
    
    // get username and password
    char* username;
    char* password;
    cout << "enter username: ";
    cin >> username;
    cout << "enter password: ";
    cin >> password;
    
    // TODO: call to server to see if U/P match
    // if doesn't match
    //  print error
    // otherwise:
    //  get user type from the server, as well as the current phase
    //  phases: 1 = authors submitting, 2 = reviewing, 3 = commenting, 4 = commenting v2, 5 = accepting
    int phase = 1;
    user currUser;
    currUser.type = 1;
    
    cout << "Welcome, " << username << endl;
    
    // TODO: maybe do this in a better way?
    switch(currUser.type)
    {
        case(1):
        {
            handleAdmin(username, phase);
            break;
        }
        case(2):
        {
            handleAuthor(username, phase);
            break;
        }
        case(3):
        {
            handleReviewer(username, phase);
            break;
        }
        default:
            cout << "An error has occurred" << endl;
    }
    
    // TODO: perform any extra bits
    
    // exit
    cout << "Goodbye!" << endl;
}

void handleAdmin(char* username, int phase)
{
    char input = '0';
    while (input != '1')
    {
        // print menu
        cout << "Menu: 0: switch phase";
        cout << "1: exit" << endl;
        
        // get input
        cin >> input;
    }
    
    // TODO: do what the user entered
}

void handleAuthor(char* username, int phase)
{
    char input = '0';
    while (input != '1')
    {
        // print menu
        cout << "Menu:";
        if (phase == 1)
            cout << "0: submit work";
        if (phase == 4)
            cout << "0: comment on reviews";
        cout << "1: exit" << endl;
        
        // get input
        cin >> input;
    }
    
    // TODO: do what the user entered
}

void handleReviewer(char* username, int phase)
{
    char input = '0';
    while (input != '1')
    {
        // print menu
        cout << "Menu:";
        if (phase == 2)
            cout << "0: submit review";
        if (phase == 3)
            cout << "0: comment on reviews";
        cout << "1: exit" << endl;
        
        // get input
        cin >> input;
    }
    
    // TODO: do what the user entered
}
