#include <iostream>
#include <stack>
#include <string>
using namespace std;

// Simple Cart Stack implementation
class CartStack {
    stack<string> carts;
public:
    void addCart(const string& cartId) {
        carts.push(cartId);
        cout << "Added cart: " << cartId << endl;
    }

    void removeCart() {
        if (carts.empty()) {
            cout << "No carts to remove." << endl;
            return;
        }
        cout << "Removed cart: " << carts.top() << endl;
        carts.pop();
    }

    void showTopCart() {
        if (carts.empty()) {
            cout << "No carts in the stack." << endl;
            return;
        }
        cout << "Top cart: " << carts.top() << endl;
    }

    bool isEmpty() const {
        return carts.empty();
    }

    int size() const {
        return carts.size();
    }
};

int main() {
    CartStack cartStack;

    cartStack.addCart("CartA");
    cartStack.addCart("CartB");
    cartStack.addCart("CartC");

    cartStack.showTopCart();

    cartStack.removeCart();
    cartStack.showTopCart();

    cout << "Stack size: " << cartStack.size() << endl;

    cartStack.removeCart();
    cartStack.removeCart();
    cartStack.removeCart(); // Should show no carts to remove

    return 0;
}
