#include "bills_window.hpp"
#include "ui_bills_window.h"

#include <streambuf>
#include <iostream>
#include <fstream>
#include <QDate>

BillsWindow::BillsWindow(QWidget *parent) :
        QWidget{parent},
        ui_{new Ui::BillsWindow} {
    ui_->setupUi(this);

    std::ifstream file{file_name_};

    bills_json_ = QJsonDocument::fromJson(QByteArray::fromStdString(std::string{
            std::istreambuf_iterator<char>{file},
            std::istreambuf_iterator<char>{}
    })).object();

    std::cout << QJsonDocument{bills_json_}.toJson().toStdString() << std::endl;

    QJsonValue rates_json = bills_json_["rates"];
    QJsonValue water_rates_json = rates_json["water"];
    rates_.water.hot = water_rates_json["hot"].toDouble();
    rates_.water.cold = water_rates_json["cold"].toDouble();
    rates_.water.drain = water_rates_json["drain"].toDouble();

//    auto &electricity_rates_json TODO
}

BillsWindow::~BillsWindow() {
    std::ofstream{file_name_} << QJsonDocument{bills_json_}.toJson().toStdString();
    delete ui_;
}

void BillsWindow::on_pushButton_clicked() {
    bool ok = false;
    dec::decimal<2> water_hot{ui_->textEdit_water_hot->toPlainText().toDouble(&ok)};
    if (!ok) { return; }

    dec::decimal<2> water_cold{ui_->textEdit_water_cold->toPlainText().toDouble(&ok)};
    if (!ok) { return; }

//    dec::decimal<2> electricity_t1{ui_->textEdit_electricity_t1->toPlainText().toDouble(&ok)};
    if (!ok) { return; }

//    dec::decimal<2> electricity_t2{ui_->textEdit_electricity_t2->toPlainText().toDouble(&ok)};
    if (!ok) { return; }

//    dec::decimal<2> electricity_t3{ui_->textEdit_electricity_t3->toPlainText().toDouble(&ok)};
    if (!ok) { return; }

    auto bills = bills_json_["bills"].toArray();
    auto last_bill = bills.last();
    auto last_water_bill = last_bill["water"];
    dec::decimal<2> last_hot_water_bill{last_water_bill["hot"].toDouble()};
    dec::decimal<2> last_cold_water_bill{last_water_bill["cold"].toDouble()};

    dec::decimal<2> sum_to_pay{
            (water_hot - last_hot_water_bill) * rates_.water.hot +
            (water_cold - last_cold_water_bill) * rates_.water.cold +
            ((water_hot - last_hot_water_bill) +
             (water_cold - last_cold_water_bill)) * rates_.water.drain
            // TODO: electricity
    };

    ui_->label_calculated->setText(QString::number(sum_to_pay.getUnbiased() / 100) + "." +
                                   QString::number(sum_to_pay.getUnbiased() % 100) +
                                   " rubles");

    QJsonObject bill;
    bill["month"] = QDate::currentDate().toString("MMMM");
    bill["sum"] = QString::number(sum_to_pay.getUnbiased() / 100) + "." +
                  QString::number(sum_to_pay.getUnbiased() % 100);
    QJsonObject bill_water;
    bill_water["hot"] = water_hot.getAsDouble();
    bill_water["cold"] = water_cold.getAsDouble();
    bill["water"] = bill_water;
    // TODO: electricity
    bills.push_back(bill);
    bills_json_["bills"] = bills;
}

// 523.89
// 681.81
