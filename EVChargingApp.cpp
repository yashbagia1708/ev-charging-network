#include "Menu.h"

int main() {
    EVCharging charging;
    int choice;

    do {
        displayMenu();
        cin >> choice;
        handleMenuChoice(charging, choice);

        cout << "Do you want to perform another operation? (y/n): ";
        char another;
        cin >> another;
        if (another == 'n' || another == 'N') {
            break;
        }

    } while (true);

    return 0;
}
