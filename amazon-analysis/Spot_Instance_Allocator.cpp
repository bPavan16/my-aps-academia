#include <iostream>
#include <string>
#include <vector>
using namespace std;

class SpotInstanceRequest {
public:
    string instanceId;
    double bidPrice;
    bool isActive;

    SpotInstanceRequest(string id, double price)
        : instanceId(id), bidPrice(price), isActive(true) {}
};

class SpotAllocator {
private:
    vector<SpotInstanceRequest> requests;
    double currentSpotPrice;

public:
    SpotAllocator(double price) : currentSpotPrice(price) {}

    void submitRequest(const string& id, double bidPrice) {
        requests.push_back(SpotInstanceRequest(id, bidPrice));
        cout << "Submitted spot request for instance: " << id 
             << " with bid price: " << bidPrice << endl;
    }

    void setCurrentSpotPrice(double newPrice) {
        currentSpotPrice = newPrice;
        cout << "Current spot price updated to: " << newPrice << endl;
        refreshRequests();
    }

    void refreshRequests() {
        // Activate if bid >= spot price, otherwise deactivate
        for (auto& req : requests) {
            if (req.bidPrice >= currentSpotPrice) {
                req.isActive = true;
            } else {
                req.isActive = false;
            }
        }
    }

    void printActiveRequests() {
        cout << "Active Spot Requests:" << endl;
        for (auto& req : requests) {
            if (req.isActive) {
                cout << " - " << req.instanceId 
                     << " (Bid: " << req.bidPrice << ")" << endl;
            }
        }
    }
};

int main() {
    SpotAllocator allocator(0.05);

    allocator.submitRequest("i-abc1234", 0.04);
    allocator.submitRequest("i-def5678", 0.06);
    allocator.printActiveRequests();

    cout << endl << "Updating spot price to 0.03..." << endl;
    allocator.setCurrentSpotPrice(0.03);
    allocator.printActiveRequests();

    cout << endl << "Updating spot price to 0.05..." << endl;
    allocator.setCurrentSpotPrice(0.05);
    allocator.printActiveRequests();

    return 0;
}
