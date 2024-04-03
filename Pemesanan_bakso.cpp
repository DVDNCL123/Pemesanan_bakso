#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

struct MeatballItem
{
    string nama;
    double harga;
};

class OrderNode
{
public:
    string namaBakso;
    int jumlah;
    double harga;
    OrderNode *next;

    OrderNode(string namaBakso, int jumlah, double harga)
        : namaBakso(namaBakso), jumlah(jumlah), harga(harga), next(nullptr) {}
};

class BaksoPalaBapakau
{
private:
    vector<MeatballItem> menuBakso;
    OrderNode *head;
    double total;

public:
    BaksoPalaBapakau() : head(nullptr), total(0.0)
    {
        menuBakso = {
            {"Bakso Biasa", 20000},
            {"Bakso Tahu", 20000},
            {"Bakso Super", 25000},
            {"Bakso Kepala Sapi", 30000},
            {"Bakso Urat", 27000},
            {"Bakso Balungan", 32000},
            {"Bakso Tulang Muda", 25000},
            {"Bakso Tethelan", 25000}};
    }

    ~BaksoPalaBapakau()
    {
        clearOrders();
    }

    void displayMeatballMenu()
    {
        cout << "==================== Menu Bakso ====================" << endl;
        cout << left << setw(4) << "No" << setw(20) << "Nama Bakso" << setw(20) << "Harga" << endl;
        cout << "-----------------------------------------------------" << endl;

        for (size_t i = 0; i < menuBakso.size(); ++i)
        {
            cout << left << setw(4) << i + 1 << setw(20) << menuBakso[i].nama << "Rp " << setw(15) << fixed << setprecision(0) << menuBakso[i].harga << endl;
        }

        cout << "=====================================================" << endl;
    }

    void tambahPesanan(int nomorPesanan, int jumlah)
    {
        if (nomorPesanan > 0 && nomorPesanan <= menuBakso.size())
        {
            const auto &item = menuBakso[nomorPesanan - 1];
            OrderNode *newNode = new OrderNode(item.nama, jumlah, item.harga * jumlah);
            total += newNode->harga;

            if (!head)
            {
                head = newNode;
            }
            else
            {
                OrderNode *temp = head;
                while (temp->next)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
        else
        {
            cout << "Pilihan tidak valid." << endl;
        }
    }

    void pesanan()
    {
        int index = 1;
        OrderNode *temp = head;
        cout << "\nCurrent Orders:" << endl;
        while (temp)
        {
            cout << index++ << ". " << temp->namaBakso << " x " << temp->jumlah << " (Rp." << fixed << setprecision(2) << temp->harga << ")" << endl;
            temp = temp->next;
        }
        cout << endl;
    }

    void modifyOrder(int nomorPesanan, int jumlahBaru)
    {
        int index = 1;
        OrderNode *temp = head;
        while (temp)
        {
            if (index == nomorPesanan)
            {
                total -= temp->harga;
                temp->jumlah = jumlahBaru;
                temp->harga = jumlahBaru * (temp->harga / temp->jumlah);
                total += temp->harga;
                cout << temp->namaBakso << " diubah menjadi " << jumlahBaru << endl;
                return;
            }
            temp = temp->next;
            index++;
        }
        cout << "Pesanan tidak ditemukan cuy!" << endl;
    }

    void clearOrders()
    {
        while (head != nullptr)
        {
            OrderNode *temp = head;
            head = head->next;
            delete temp;
        }
        total = 0.0;
    }

    int getMenuSize() const
    {
        return menuBakso.size();
    }

    void checkout(const string &namaPembeli, const string &metodePembayaran)
    {
        cout << "\n==================== Nota Pembelian ====================" << endl;
        cout << "Atas Nama          : " << namaPembeli << endl;
        cout << "Metode Pembayaran  : " << metodePembayaran << endl;
        cout << "" << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << left << setw(20) << "Nama Bakso" << setw(20) << "jumlah" << setw(15) << "Harga" << endl;
        cout << "---------------------------------------------------------" << endl;

        OrderNode *temp = head;
        while (temp)
        {
            cout << left << setw(23) << temp->namaBakso << setw(15) << temp->jumlah << setw(15) << fixed << setprecision(0) << temp->harga << endl;
            temp = temp->next;
        }
        cout << "---------------------------------------------------------" << endl;
        cout << "Total Pembayaran : Rp " << fixed << setprecision(0) << total << endl;
        cout << "=========================================================" << endl;
        cout << "         *** Terimakasih Atas Kunjungan Anda ***         " << endl;
        cout << "" << endl;

        ofstream file("transactions.txt", ios::app);
        file << "Customer: " << namaPembeli << ", Total: Rp " << total << ", pembayaran: " << metodePembayaran << endl;
        file.close();
    }
};

int main()
{
    BaksoPalaBapakau shop;
    shop.displayMeatballMenu();

    string namaPembeli;
    cout << "Atas nama : ";
    getline(cin, namaPembeli);

    int nomorPesanan, jumlah;
    while (true)
    {
        cout << "(ketik 0 untuk menyelesaikan pemesanan, ketik -1 untuk mengubah pesanan)" << endl;
        cout << " Masukkan nomor yang ingin anda pesan : ";
        cin >> nomorPesanan;

        if (nomorPesanan == 0)
        {
            break;
        }
        else if (nomorPesanan == -1)
        {
            shop.pesanan();
            int nomorPesanan, jumlahBaru;
            cout << "Pilih pesanan yang ingin diubah: ";
            cin >> nomorPesanan;
            cout << "Masukkan jumlah baru: ";
            cin >> jumlahBaru;
            shop.modifyOrder(nomorPesanan, jumlahBaru);
            continue;
        }

        if (nomorPesanan < 0 || nomorPesanan > shop.getMenuSize())
        {
            cout << "Nomor tidak valid! Silakan coba lagi yaa." << endl;
            continue;
        }

        cout << " Masukkan jumlah: ";
        cin >> jumlah;

        if (jumlah <= 0)
        {
            cout << "Jumlah tidak valid! silakan masukkan jumlah yang positif." << endl;
            continue;
        }

        shop.tambahPesanan(nomorPesanan, jumlah);
    }

    string metodePembayaran;
    cout << "Pilih metode pembayaran (Cash/Card/Transfer/Qris): ";
    cin >> metodePembayaran;

    shop.checkout(namaPembeli, metodePembayaran);

    return 0;
} 