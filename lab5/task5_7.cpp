#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

class Product {
private:
    string name;
    int id;
    double weight;
    double price;
    int shelf_life;
public:
    int get_id() const {
        return id;
    }
    string get_name() const {
        return name;
    }
    double get_weight() const {
        return weight;
    }
    double get_price() const {
        return price;
    }
    int get_shelf_life() const {
        return shelf_life;
    }
    Product(const string& name, int id, double weight, double price, int shelf_life)
        : name(name), id(id), weight(weight), price(price), shelf_life(shelf_life) {}
    Product(const Product& p)
        : name(p.name), id(p.id), weight(p.weight), price(p.price), shelf_life(p.shelf_life) {}
    Product& operator=(const Product& p) {
        if (this != &p) {
            name = p.name;
            id = p.id;
            weight = p.weight;
            price = p.price;
            shelf_life = p.shelf_life;
        }
        return *this;
    }
    virtual ~Product() = default;
    virtual void displayInfo() const {
        cout << "Name: " << name
            << ", ID: " << id
            << ", Weight: " << weight << "kg"
            << ", Price: " << price
            << ", Shelf Life: " << shelf_life << " days" << endl;
    }
    virtual double calculateStorageFee() const {
        return weight * 500;
    }
};

class PerishableProduct : public Product {
private:
    time_t expiration_date;
public:
    time_t get_date() const {
        return expiration_date;
    }
    PerishableProduct(const string& name, int id, double weight, double price, int shelf_life, time_t e_date)
        : Product(name, id, weight, price, shelf_life), expiration_date(e_date) {}
    double calculateStorageFee() const override {
        time_t now = time(0);
        double rest_days = difftime(expiration_date, now) / (60 * 60 * 24);
        if (rest_days < 0)
            return 0;
        return 1000000 / rest_days * get_weight();
    }
    void displayInfo() const override {
        Product::displayInfo();
        cout << "Expiration Date: " << ctime(&expiration_date);
    }
};

class ElectronicProduct : public Product {
private:
   int warranty_period;
   double power_rating;
public:
    int get_war_period() const {
        return warranty_period;
    }
    double get_pow_rate() const {
        return power_rating;
    }
    ElectronicProduct(const string& name, int id, double weight, double price, int shelf_life, int warranty_period, double power_rating)
        : Product(name, id, weight, price, shelf_life), warranty_period(warranty_period), power_rating(power_rating) {}
    void displayInfo() const override {
        Product::displayInfo();
        cout << "Warranty Period: " << warranty_period << " days"
            << ", Power Rating: " << power_rating << "W" << endl;
    }
};

class BuildingMaterial : public Product {
private:
    bool flammability;
public:
    bool get_flammability() {
        return flammability;
    }
    BuildingMaterial(const string& name, int id, double weight, double price, int shelf_life, bool flammability)
        : Product(name, id, weight, price, shelf_life), flammability(flammability) {}
    double calculateStorageFee() const override {
        return get_weight() * (flammability ? 780 : 240);
    }
    void displayInfo() const override {
        Product::displayInfo();
        if (flammability)
            cout << "Flammable" << endl;
        else
            cout << "Not flammable" << endl;
    }
};

class Warehouse {
private:
    vector<shared_ptr<Product>> products;
    double calculateTotalStorageFee() const {
        double res = 0;
        for (const auto& product : products) 
            res += product->calculateStorageFee();
        return res;
    }
public:
    void append(shared_ptr<Product> product) {
        products.push_back(product);
    }
    void remove_prod(int id) {
        int sz = products.size();
        for (int i = 0; i < sz; i++){
            if (products[i]->get_id() == id){
                products.erase(products.begin() + i);
                break;
            }
        }
    }
    shared_ptr<Product> find_product(int id) const {
        for (const auto& product : products)
            if (product->get_id() == id) return product;
        return nullptr;
    }
    vector<shared_ptr<Product>> getExpiringProducts(int days) const {
        vector<shared_ptr<Product>> res;
        time_t now = time(0);
        for (const auto& product : products) {
            auto perishable = dynamic_pointer_cast<PerishableProduct>(product);
            if (perishable) {
                double rest_days = difftime(perishable->get_date(), now) / (60 * 60 * 24);
                if (rest_days <= days) 
                    res.push_back(perishable);
            }
        }
        return res;
    }
    void displayInventory() const {
        map<string, vector<shared_ptr<Product>>> categories;
        for (const auto& product : products) {
            if (dynamic_pointer_cast<PerishableProduct>(product)) 
                categories["Perishable"].push_back(product);
            else if (dynamic_pointer_cast<ElectronicProduct>(product)) 
                categories["Electronics"].push_back(product);
            else if (dynamic_pointer_cast<BuildingMaterial>(product)) 
                categories["Building Materials"].push_back(product);
        }
        for (const auto& category : categories) {
            cout << "Category: " << category.first << endl;
            for (const auto& product : category.second)
                product->displayInfo();
        }
    }
    Warehouse& operator+=(shared_ptr<Product> product) {
        append(product);
        return *this;
    }
    Warehouse& operator-=(int id) {
        remove_prod(id);
        return *this;
    }
    shared_ptr<Product> operator[](int id) const {
        return find_product(id);
    }
    friend ostream& operator<<(ostream& os, const Warehouse& warehouse) {
        os << "Products in warehouse:" << endl;
        warehouse.displayInventory();
        os << "Total Storage Fee: " << warehouse.calculateTotalStorageFee() << endl;
        return os;
    } 
};


int main()
{
    Warehouse warehouse;

    time_t now = time(0);
    time_t expiration = now + 5 * 24 * 60 * 60;

    warehouse += make_shared<ElectronicProduct>("Smartphone", 54, 3, 30000, 0, 24, 65.0);
    warehouse += make_shared<PerishableProduct>("Fish", 21, 1.5, 900, 7, expiration);
    warehouse += make_shared<BuildingMaterial>("Wood_boards", 33, 100, 8000, 0, true);

    cout << warehouse << endl << endl;

    auto expiringProducts = warehouse.getExpiringProducts(7);
    cout << "Expiring Products:" << endl;
    for (const auto& product : expiringProducts)
        product->displayInfo();

    cout << endl << endl;

    warehouse -= 54;
    cout << warehouse;

    return 0;
}