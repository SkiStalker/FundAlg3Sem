#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <map>

using namespace std;

class Product {
private:
    string name;
    int id;
    double weight;
    double price;
    int shelfLife;

public:
    Product(const string& name, int id, double weight, double price, int shelfLife)
        : name(name), id(id), weight(weight), price(price), shelfLife(shelfLife) {}

    Product(const Product& other)
        : name(other.name), id(other.id), weight(other.weight), price(other.price), shelfLife(other.shelfLife) {}

    virtual ~Product() = default;

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

    virtual void displayInfo() const {
        cout << "Name: " << name
             << ", ID: " << id
             << ", Weight: " << weight << "kg"
             << ", Price: $" << price
             << ", Shelf Life: " << shelfLife << " days" << endl;
    }

    virtual double calculateStorageFee() const {
        return weight * 0.1;
    }

    int getId() const {
        return id; 
    }
    string getName() const {
        return name;
    }
    double getWeight() const {
        return weight;
    }
    double getPrice() const {
        return price;
    }
    int getShelLife() const {
        return shelfLife;
    }
};

class PerishableProduct : public Product {
private:
    time_t expirationDate;

public:
    PerishableProduct(const string& name, int id, double weight, double price, int shelfLife, time_t expirationDate)
        : Product(name, id, weight, price, shelfLife), expirationDate(expirationDate) {}

    double calculateStorageFee() const override {
        time_t now = time(0);
        double daysToExpire = difftime(expirationDate, now) / (60 * 60 * 24);
        if (daysToExpire < 0) daysToExpire = 0;
        return getWeight() * (0.2 + (1.0 / (daysToExpire + 1)));
    }

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Expiration Date: " << ctime(&expirationDate);
    }

    time_t getExpirationDate() const {
        return expirationDate;
    }
};

class BuildingMaterial : public Product {
private:
    bool flammable;

public:
    BuildingMaterial(const string& name, int id, double weight, double price, int shelfLife, bool flammable)
        : Product(name, id, weight, price, shelfLife), flammable(flammable) {}

    double calculateStorageFee() const override {
        return getWeight() * (flammable ? 0.5 : 0.2);
    }

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Flammable: " << (flammable ? "Yes" : "No") << endl;
    }

    bool getFlammable() {
        return flammable;
    }
};

class ElectronicProduct : public Product {
private:
    int warrantyPeriod;
    double powerRating;

public:
    ElectronicProduct(const string& name, int id, double weight, double price, int shelfLife, int warrantyPeriod, double powerRating)
        : Product(name, id, weight, price, shelfLife), warrantyPeriod(warrantyPeriod), powerRating(powerRating) {}

    void displayInfo() const override {
        Product::displayInfo();
        cout << "Warranty Period: " << warrantyPeriod << " months"
             << ", Power Rating: " << powerRating << "W" << endl;
    }

    int getWarrantyPeriod() const {
        return warrantyPeriod;
    }
    double getPowerRating() const {
        return powerRating;
    }
};

class Warehouse {
private:
    vector<shared_ptr<Product>> inventory;

public:
    void addProduct(shared_ptr<Product> product) {
        inventory.push_back(product);
    }

    void removeProduct(int id) {
        inventory.erase(remove_if(inventory.begin(), inventory.end(),
                                  [id](const shared_ptr<Product>& product) {
                                      return product->getId() == id;
                                  }),
                        inventory.end());
    }

    shared_ptr<Product> findProduct(int id) const {
        for (const auto& product : inventory) {
            if (product->getId() == id) return product;
        }
        return nullptr;
    }

    vector<shared_ptr<Product>> getExpiringProducts(int days) const {
        vector<shared_ptr<Product>> expiring;
        time_t now = time(0);
        for (const auto& product : inventory) {
            auto perishable = dynamic_pointer_cast<PerishableProduct>(product);
            if (perishable) {
                double remainingDays = difftime(perishable->getExpirationDate(), now) / (60 * 60 * 24);
                if (remainingDays <= days) {
                    expiring.push_back(perishable);
                }
            }
        }
        return expiring;
    }

    void displayInventory() const {
        map<string, vector<shared_ptr<Product>>> categories;
        for (const auto& product : inventory) {
            if (dynamic_pointer_cast<PerishableProduct>(product)) {
                categories["Perishable"].push_back(product);
            } else if (dynamic_pointer_cast<ElectronicProduct>(product)) {
                categories["Electronics"].push_back(product);
            } else if (dynamic_pointer_cast<BuildingMaterial>(product)) {
                categories["Building Materials"].push_back(product);
            }
        }

        for (const auto& [category, products] : categories) {
            cout << "Category: " << category << endl;
            for (const auto& product : products) {
                product->displayInfo();
            }
        }
    }

    Warehouse& operator+=(shared_ptr<Product> product) {
        addProduct(product);
        return *this;
    }

    Warehouse& operator-=(int id) {
        removeProduct(id);
        return *this;
    }

    shared_ptr<Product> operator[](int id) const {
        return findProduct(id);
    }

    friend ostream& operator<<(ostream& os, const Warehouse& warehouse) {
        os << "Warehouse Inventory:" << endl;
        warehouse.displayInventory();
        os << "Total Storage Fee: $" << warehouse.calculateTotalStorageFee() << endl;
        return os;
    }

private:
    double calculateTotalStorageFee() const {
        double total = 0;
        for (const auto& product : inventory) {
            total += product->calculateStorageFee();
        }
        return total;
    }
};

int main() {
    Warehouse warehouse;

    time_t now = time(0);
    time_t expiration = now + 5 * 24 * 60 * 60;

    warehouse += make_shared<PerishableProduct>("Milk", 1, 2.0, 1.5, 7, expiration);
    warehouse += make_shared<ElectronicProduct>("Laptop", 2, 3.0, 1500.0, 0, 24, 65.0);
    warehouse += make_shared<BuildingMaterial>("Bricks", 3, 100.0, 500.0, 0, true);

    cout << warehouse << endl << "-----------------------------------------" << endl;

    auto expiringProducts = warehouse.getExpiringProducts(7);
    cout << "Expiring Products:" << endl;
    for (const auto& product : expiringProducts) {
        product->displayInfo();
    }

    cout << endl << "-----------------------------------------" << endl;

    warehouse -= 1;
    cout << warehouse;

    return 0;
}