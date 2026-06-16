#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cctype>

using namespace std;

/* ================= CONVERT TO UPPERCASE ================= */

string toUpperCase(string str) {

    for (char &c : str)
        c = toupper(c);

    return str;
}

/* ================= HISTORY CLASS ================= */

class HistoryEntry {

public:
    string action;

    HistoryEntry(string act) {
        action = act;
    }
};

/* ================= ABSTRACT ASSET ================= */

class Asset {

protected:

    string symbol;
    int quantity;
    double buyPrice;
    double currentPrice;

public:

    Asset(
        string sym,
        int qty,
        double bp,
        double cp
    ) {

        symbol = sym;
        quantity = qty;
        buyPrice = bp;
        currentPrice = cp;
    }

    virtual double calculateValue() const = 0;

    virtual void display() const {

        cout << "\nSymbol: " << symbol
             << "\nQuantity: " << quantity
             << "\nBuy Price: " << buyPrice
             << "\nCurrent Price: " << currentPrice;
    }

    virtual double profitLoss() const {

        return (currentPrice - buyPrice)
               * quantity;
    }

    string getSymbol() const {
        return symbol;
    }

    int getQuantity() const {
        return quantity;
    }

    void reduceQuantity(int qty) {
        quantity -= qty;
    }

    void updatePrice(double newPrice) {
        currentPrice = newPrice;
    }

    double getCurrentPrice() const {
        return currentPrice;
    }

    virtual string getType() const = 0;

    virtual ~Asset() {}
};

/* ================= EQUITY STOCK ================= */

class EquityStock : public Asset {

public:

    EquityStock(
        string sym,
        int qty,
        double bp,
        double cp
    ) : Asset(sym, qty, bp, cp) {}

    double calculateValue() const override {

        return quantity * currentPrice;
    }

    void display() const override {

        cout << "\n[Equity Stock]";

        Asset::display();

        cout << "\nCurrent Value: "
             << calculateValue();

        cout << "\nProfit/Loss: "
             << profitLoss() << "\n";
    }

    string getType() const override {
        return "Equity";
    }
};

/* ================= MUTUAL FUND ================= */

class MutualFund : public Asset {

public:

    MutualFund(
        string sym,
        int qty,
        double bp,
        double cp
    ) : Asset(sym, qty, bp, cp) {}

    double calculateValue() const override {

        return quantity *
               currentPrice * 0.98;
    }

    void display() const override {

        cout << "\n[Mutual Fund]";

        Asset::display();

        cout << "\nCurrent Value: "
             << calculateValue();

        cout << "\nProfit/Loss: "
             << profitLoss() << "\n";
    }

    string getType() const override {
        return "MutualFund";
    }
};

/* ================= CRYPTO ================= */

class Crypto : public Asset {

public:

    Crypto(
        string sym,
        int qty,
        double bp,
        double cp
    ) : Asset(sym, qty, bp, cp) {}

    double calculateValue() const override {

        return quantity *
               currentPrice * 1.05;
    }

    void display() const override {

        cout << "\n[Crypto Asset]";

        Asset::display();

        cout << "\nCurrent Value: "
             << calculateValue();

        cout << "\nProfit/Loss: "
             << profitLoss() << "\n";
    }

    string getType() const override {
        return "Crypto";
    }
};

/* ================= MARKET DATA ================= */

map<string, double> marketPrice = {

    {"AAPL", 180},
    {"TSLA", 220},
    {"GOOGL", 145},
    {"AMZN", 170},
    {"MSFT", 410},
    {"BTC", 60000},
    {"ETH", 3000}
};

/* ================= PORTFOLIO ================= */

class Portfolio {

private:

    vector<Asset*> assets;
    vector<HistoryEntry> history;

public:

    void addHistory(string msg) {

        time_t now = time(0);

        string dt = ctime(&now);

        dt.pop_back();

        history.push_back(
            HistoryEntry(
                msg + " at " + dt
            )
        );

        if (history.size() > 30)
            history.erase(history.begin());
    }

    void addAsset(Asset* asset) {

        assets.push_back(asset);

        addHistory(
            "Bought " +
            asset->getSymbol()
        );

        cout << "Purchase Successful!\n";
    }

    void displayPortfolio() const {

        if (assets.empty()) {

            cout << "\nPortfolio Empty!\n";

            return;
        }

        cout << "\n========== PORTFOLIO ==========\n";

        for (auto asset : assets)
            asset->display();
    }

    void showTotalProfitLoss() const {

        double total = 0;

        for (auto asset : assets)
            total += asset->profitLoss();

        cout << "\nTotal Profit/Loss: "
             << total << endl;
    }

    void searchAsset(string sym) const {

        for (auto asset : assets) {

            if (asset->getSymbol() == sym) {

                cout << "\nAsset Found!\n";

                asset->display();

                return;
            }
        }

        cout << "\nAsset Not Found!\n";
    }

    void sellAsset(
        string sym,
        int qty
    ) {

        for (auto it = assets.begin();
             it != assets.end();
             it++) {

            if ((*it)->getSymbol() == sym) {

                if (qty >
                    (*it)->getQuantity()) {

                    cout << "Insufficient Quantity!\n";

                    return;
                }

                (*it)->reduceQuantity(qty);

                addHistory(
                    "Sold " + sym
                );

                cout << "Sale Successful!\n";

                if ((*it)->getQuantity() == 0) {

                    delete *it;

                    assets.erase(it);
                }

                return;
            }
        }

        cout << "Asset Not Found!\n";
    }

    void updateAssetPrice(
        string sym,
        double price
    ) {

        for (auto asset : assets) {

            if (asset->getSymbol() == sym) {

                asset->updatePrice(price);

                marketPrice[sym] = price;

                addHistory(
                    "Updated Price of " + sym
                );

                cout << "Price Updated Successfully!\n";

                return;
            }
        }

        cout << "Asset Not Found!\n";
    }

    void bestPerformer() const {

        if (assets.empty()) {

            cout << "Portfolio Empty!\n";

            return;
        }

        Asset* best = assets[0];

        for (auto asset : assets) {

            if (asset->profitLoss() >
                best->profitLoss())

                best = asset;
        }

        cout << "\n===== BEST PERFORMER =====\n";

        best->display();
    }

    void worstPerformer() const {

        if (assets.empty()) {

            cout << "Portfolio Empty!\n";

            return;
        }

        Asset* worst = assets[0];

        for (auto asset : assets) {

            if (asset->profitLoss() <
                worst->profitLoss())

                worst = asset;
        }

        cout << "\n===== WORST PERFORMER =====\n";

        worst->display();
    }

    static bool compareProfit(
        Asset* a,
        Asset* b
    ) {

        return a->profitLoss() >
               b->profitLoss();
    }

    void sortByProfit() {

        sort(
            assets.begin(),
            assets.end(),
            compareProfit
        );

        cout << "Portfolio Sorted by Profit!\n";
    }

    /* ================= ABSTRACTED MARKET SIMULATION ================= */

    void simulateMarket() {

        srand(time(0));

        /* UPDATE ENTIRE MARKET */

        for (auto &stock : marketPrice) {

            double oldPrice =
                stock.second;

            double change =
                (rand() % 21) - 10;

            double newPrice =
                oldPrice + change;

            if (newPrice < 1)
                newPrice = 1;

            stock.second = newPrice;
        }

        /* UPDATE PORTFOLIO ASSETS */

        for (auto asset : assets) {

            asset->updatePrice(
                marketPrice[
                    asset->getSymbol()
                ]
            );
        }

        addHistory(
            "Market Simulation Executed"
        );

        cout << "\nMarket Prices Updated Successfully!\n";
    }

    void dashboardSummary() const {

        double totalProfit = 0;
        double totalValue = 0;

        for (auto asset : assets) {

            totalProfit +=
                asset->profitLoss();

            totalValue +=
                asset->calculateValue();
        }

        cout << "\n========== DASHBOARD ==========\n";

        cout << "Total Assets: "
             << assets.size() << endl;

        cout << "Portfolio Value: "
             << totalValue << endl;

        cout << "Net Profit/Loss: "
             << totalProfit << endl;
    }

    void saveToFile() {

        ofstream fout(
            "portfolio.txt"
        );

        for (auto asset : assets) {

            fout << asset->getType()
                 << " "
                 << asset->getSymbol()
                 << " "
                 << asset->getQuantity()
                 << " "
                 << asset->getCurrentPrice()
                 << endl;
        }

        fout.close();

        cout << "Portfolio Saved to File!\n";
    }

    void showHistory() const {

        cout << "\n====== LAST TRANSACTIONS ======\n";

        for (auto h : history)
            cout << h.action << endl;
    }

    ~Portfolio() {

        for (auto asset : assets)
            delete asset;
    }
};

/* ================= MARKET DISPLAY ================= */

void displayMarketStocks() {

    cout << "\n========= MARKET STOCKS =========\n";

    for (auto x : marketPrice) {

        cout << x.first
             << " -> $"
             << fixed
             << setprecision(2)
             << x.second
             << endl;
    }
}

/* ================= MULTIPLE USER LOGIN ================= */

bool login() {

    int choice;

    cout << "\n========== USER SYSTEM ==========\n";

    cout << "1. Register\n";
    cout << "2. Login\n";

    cout << "Enter Choice: ";
    cin >> choice;

    /* ================= REGISTER ================= */

    if (choice == 1) {

        string username;
        string password;

        cout << "\nCreate Username: ";
        cin >> username;

        cout << "Create Password: ";
        cin >> password;

        ofstream fout(
            "users.txt",
            ios::app
        );

        fout << username
             << " "
             << password
             << endl;

        fout.close();

        cout << "\nRegistration Successful!\n";
        cout << "Please Login Again.\n";

        return false;
    }

    /* ================= LOGIN ================= */

    else if (choice == 2) {

        string username;
        string password;

        cout << "\nUsername: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;

        ifstream fin(
            "users.txt"
        );

        string fileUser;
        string filePass;

        while (fin >>
               fileUser >>
               filePass) {

            if (username == fileUser &&
                password == filePass) {

                cout << "\nLogin Successful!\n";

                fin.close();

                return true;
            }
        }

        fin.close();

        cout << "\nInvalid Credentials!\n";

        return false;
    }

    cout << "\nInvalid Choice!\n";

    return false;
}

/* ================= MAIN ================= */

int main() {

    while (!login()) {

        cout << "\nTry Again...\n";
    }

    Portfolio portfolio;

    int choice;

    do {

        cout << "\n\n========= ADVANCED STOCK PORTFOLIO SYSTEM =========\n";

        cout << "1. View Market Stocks\n";
        cout << "2. Buy Equity Stock\n";
        cout << "3. Buy Mutual Fund\n";
        cout << "4. Buy Crypto\n";
        cout << "5. Display Portfolio\n";
        cout << "6. Show Profit/Loss\n";
        cout << "7. Search Asset\n";
        cout << "8. Sell Asset\n";
        cout << "9. Update Asset Price\n";
        cout << "10. Sort Portfolio by Profit\n";
        cout << "11. Best Performer\n";
        cout << "12. Worst Performer\n";
        cout << "13. Simulate Market\n";
        cout << "14. Dashboard Summary\n";
        cout << "15. Save Portfolio to File\n";
        cout << "16. Transaction History\n";
        cout << "17. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        try {

            switch (choice) {

            case 1:
                displayMarketStocks();
                break;

            case 2: {

                string sym;
                int qty;
                double buyP;

                cout << "Enter Stock Symbol: ";
                cin >> sym;

                sym = toUpperCase(sym);

                cout << "Enter Quantity: ";
                cin >> qty;

                if (qty <= 0)
                    throw "Invalid Quantity!";

                cout << "Enter Buy Price: ";
                cin >> buyP;

                if (marketPrice.find(sym)
                    == marketPrice.end()) {

                    cout << "Stock Not Available!\n";

                    break;
                }

                portfolio.addAsset(
                    new EquityStock(
                        sym,
                        qty,
                        buyP,
                        marketPrice[sym]
                    )
                );

                break;
            }

            case 3: {

                string sym;
                int qty;
                double bp, cp;

                cout << "Enter Fund Symbol: ";
                cin >> sym;

                sym = toUpperCase(sym);

                cout << "Enter Units: ";
                cin >> qty;

                if (qty <= 0)
                    throw "Invalid Units!";

                cout << "Enter Buy NAV: ";
                cin >> bp;

                cout << "Enter Current NAV: ";
                cin >> cp;

                portfolio.addAsset(
                    new MutualFund(
                        sym,
                        qty,
                        bp,
                        cp
                    )
                );

                break;
            }

            case 4: {

                string sym;
                int qty;
                double bp;

                cout << "Enter Crypto Symbol: ";
                cin >> sym;

                sym = toUpperCase(sym);

                cout << "Enter Quantity: ";
                cin >> qty;

                if (qty <= 0)
                    throw "Invalid Quantity!";

                cout << "Enter Buy Price: ";
                cin >> bp;

                if (marketPrice.find(sym)
                    == marketPrice.end()) {

                    cout << "Crypto Not Available!\n";

                    break;
                }

                portfolio.addAsset(
                    new Crypto(
                        sym,
                        qty,
                        bp,
                        marketPrice[sym]
                    )
                );

                break;
            }

            case 5:
                portfolio.displayPortfolio();
                break;

            case 6:
                portfolio.showTotalProfitLoss();
                break;

            case 7: {

                string sym;

                cout << "Enter Symbol to Search: ";
                cin >> sym;

                sym = toUpperCase(sym);

                portfolio.searchAsset(sym);

                break;
            }

            case 8: {

                string sym;
                int qty;

                cout << "Enter Symbol to Sell: ";
                cin >> sym;

                sym = toUpperCase(sym);

                cout << "Enter Quantity to Sell: ";
                cin >> qty;

                portfolio.sellAsset(sym, qty);

                break;
            }

            case 9: {

                string sym;
                double price;

                cout << "Enter Symbol: ";
                cin >> sym;

                sym = toUpperCase(sym);

                cout << "Enter New Price: ";
                cin >> price;

                portfolio.updateAssetPrice(
                    sym,
                    price
                );

                break;
            }

            case 10:
                portfolio.sortByProfit();
                break;

            case 11:
                portfolio.bestPerformer();
                break;

            case 12:
                portfolio.worstPerformer();
                break;

            case 13:
                portfolio.simulateMarket();
                break;

            case 14:
                portfolio.dashboardSummary();
                break;

            case 15:
                portfolio.saveToFile();
                break;

            case 16:
                portfolio.showHistory();
                break;

            case 17:
                cout << "Thank You for Using Portfolio Manager!\n";
                break;

            default:
                cout << "Invalid Choice!\n";
            }
        }

        catch (const char* msg) {

            cout << msg << endl;
        }

    } while (choice != 17);

    return 0;
}