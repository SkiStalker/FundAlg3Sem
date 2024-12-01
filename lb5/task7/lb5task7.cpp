#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <ctime>

using namespace std;

class Product {
protected:
    string name;
    int id;
    double weight;
    double price;
    int shelfLife; 

public:
    Product(string name, int id, double weight, double price, int shelfLife)
        : name(name), id(id), weight(weight), price(price), shelfLife(shelfLife) {}

    virtual ~Product() {}

    virtual double calculateStorageFee() const {
        return weight * 0.1; 
    }


    virtual void displayInfo() const {
        cout << "Product: " << name << ", ID: " << id << ", Weight: " << weight
            << "kg, Price: $" << price << ", Shelf Life: " << shelfLife << " days." << endl;
    }

    int getId() const {
        return id;
    }
    Product(const Product& other)
        : name(other.name), id(other.id), weight(other.weight), price(other.price), shelfLife(other.shelfLife) {}

    Product& operator=(const Product& other) {
        if (this != &other) {
            name = other.name;
            id = other.id;
            weight = other.weight;
            price = other.price;
            shelfLife = other.shelfLife;
        }
        return *this;
    }
};

class PerishableProduct : public Product {
private:
    time_t expirationDate; 

public:
    PerishableProduct(string name, int id, double weight, double price, int shelfLife, time_t expirationDate)
        : Product(name, id, weight, price, shelfLife), expirationDate(expirationDate) {}

    time_t getExpirationDate() const {
        return expirationDate;
    }

    
    double calculateStorageFee() const override {
        time_t now = time(0);
        double fee = Product::calculateStorageFee();
        if (difftime(expirationDate, now) < 7 * 24 * 60 * 60) { 
            fee *= 2; 
        }
        return fee;
    }

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Expiration Date: " << ctime(&expirationDate);
    }
};

class ElectronicProduct : public Product {
private:
    int warrantyPeriod; 
    double powerRating;

public:
    ElectronicProduct(string name, int id, double weight, double price, int shelfLife, int warrantyPeriod, double powerRating)
        : Product(name, id, weight, price, shelfLife), warrantyPeriod(warrantyPeriod), powerRating(powerRating) {}

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Warranty Period: " << warrantyPeriod << " months, Power Rating: " << powerRating << "W." << endl;
    }
};

class BuildingMaterial : public Product {
private:
    bool flammability; 

public:
    BuildingMaterial(string name, int id, double weight, double price, int shelfLife, bool flammability)
        : Product(name, id, weight, price, shelfLife), flammability(flammability) {}

    double calculateStorageFee() const override {
        double fee = Product::calculateStorageFee();
        if (flammability) {
            fee *= 1.5; 
        }
        return fee;
    }

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Flammability: " << (flammability ? "Yes" : "No") << endl;
    }
};


class Warehouse {
private:
    vector<shared_ptr<Product>> inventory; 

public:
    void operator+=(shared_ptr<Product> product) {
        inventory.push_back(product);
    }

    void operator-=(int id) {
        auto it = remove_if(inventory.begin(), inventory.end(), [id](const shared_ptr<Product>& product) {
            return product->getId() == id;
            });
        inventory.erase(it, inventory.end());
    }

    shared_ptr<Product> operator[](int id) {
        for (auto& product : inventory) {
            if (product->getId() == id) {
                return product;
            }
        }
        return nullptr;
    }

    double calculateTotalStorageFee() const {
        double totalFee = 0;
        for (const auto& product : inventory) {
            totalFee += product->calculateStorageFee();
        }
        return totalFee;
    }

    vector<shared_ptr<Product>> getExpiringProducts(int days) const {
        vector<shared_ptr<Product>> expiringProducts;
        time_t now = time(0);
        for (const auto& product : inventory) {
            if (auto pProduct = dynamic_cast<PerishableProduct*>(product.get())) {
                double diff = difftime(pProduct->getExpirationDate(), now) / (60 * 60 * 24);
                if (diff <= days) {
                    expiringProducts.push_back(product);
                }
            }
        }
        return expiringProducts;
    }

    void displayInventory() const {
        for (const auto& product : inventory) {
            product->displayInfo();
            cout << "Storage Fee: $" << product->calculateStorageFee() << endl;
            cout << "----------------------------" << endl;
        }
    }
};


int main() {
    Warehouse warehouse;

    auto product1 = make_shared<PerishableProduct>("Milk", 1, 1.0, 2.5, 7, time(0) + 3 * 24 * 60 * 60);
    auto product2 = make_shared<ElectronicProduct>("TV", 2, 8.0, 299.99, 365, 24, 150);
    auto product3 = make_shared<BuildingMaterial>("Wood Plank", 3, 10.0, 50.0, 180, true);

    warehouse += product1;
    warehouse += product2;
    warehouse += product3;

    warehouse.displayInventory();

    cout << "Total Storage Fee: $" << warehouse.calculateTotalStorageFee() << endl;

    return 0;
}
