#pragma once

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <filesystem>

#include "thirdparty/decimal.hpp"

struct Rates {
    struct Water {
        dec::decimal<2> hot;
        dec::decimal<2> cold;
        dec::decimal<2> drain;
    };
    Water water;
    struct Electricity {
        dec::decimal<2> t1;
        dec::decimal<2> t2;
        dec::decimal<2> t3;
    };
    Electricity electricity;
};

namespace Ui {
class BillsWindow;
}

class BillsWindow : public QWidget {
 Q_OBJECT

 public:
    explicit BillsWindow(QWidget *parent = nullptr);
    ~BillsWindow() override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::BillsWindow *ui_;
    std::string file_name_{std::filesystem::current_path() /= "../bills.json"};
    QJsonObject bills_json_{};
    Rates rates_;
};
