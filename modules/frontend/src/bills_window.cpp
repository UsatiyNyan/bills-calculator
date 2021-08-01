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

    QJsonValue electricity_rates_json = rates_json["electricity"];
    rates_.electricity.t1 = electricity_rates_json["t1"].toDouble();
    rates_.electricity.t2 = electricity_rates_json["t2"].toDouble();
    rates_.electricity.t3 = electricity_rates_json["t3"].toDouble();
}

BillsWindow::~BillsWindow() {
    std::ofstream{file_name_} << QJsonDocument{bills_json_}.toJson().toStdString();
    delete ui_;
}

void BillsWindow::on_pushButton_clicked() {
    bool ok = false;
    dec::decimal<2> water_hot{ui_->textEdit_water_hot->toPlainText().toDouble(&ok)};
    if (!ok) {
        return;
    }

    dec::decimal<2> water_cold{ui_->textEdit_water_cold->toPlainText().toDouble(&ok)};
    if (!ok) {
        return;
    }

    dec::decimal<2> electricity_t1{ui_->textEdit_electricity_t1->toPlainText().toDouble(&ok)};
    if (!ok) {
        return;
    }

    dec::decimal<2> electricity_t2{ui_->textEdit_electricity_t2->toPlainText().toDouble(&ok)};
    if (!ok) {
        return;
    }

    dec::decimal<2> electricity_t3{ui_->textEdit_electricity_t3->toPlainText().toDouble(&ok)};
    if (!ok) {
        return;
    }

    auto bills = bills_json_["bills"].toArray();
    auto last_bill = bills.last();
    auto last_water_bill = last_bill["water"];
    dec::decimal<2> last_hot_water_bill{last_water_bill["hot"].toDouble()};
    dec::decimal<2> last_cold_water_bill{last_water_bill["cold"].toDouble()};

    auto last_electricity_bill = last_bill["electricity"];
    dec::decimal<2> last_electricity_t1_bill{last_electricity_bill["t1"].toDouble()};
    dec::decimal<2> last_electricity_t2_bill{last_electricity_bill["t2"].toDouble()};
    dec::decimal<2> last_electricity_t3_bill{last_electricity_bill["t3"].toDouble()};

    dec::decimal<2> sum_to_pay{
            (water_hot - last_hot_water_bill) * rates_.water.hot +
            (water_cold - last_cold_water_bill) * rates_.water.cold +
            ((water_hot - last_hot_water_bill) + (water_cold - last_cold_water_bill)) * rates_.water.drain +
            (electricity_t1 - last_electricity_t1_bill) * rates_.electricity.t1 +
            (electricity_t2 - last_electricity_t2_bill) * rates_.electricity.t2 +
            (electricity_t3 - last_electricity_t3_bill) * rates_.electricity.t3
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

    QJsonObject bill_electricity;
    bill_electricity["t1"] = electricity_t1.getAsDouble();
    bill_electricity["t2"] = electricity_t2.getAsDouble();
    bill_electricity["t3"] = electricity_t3.getAsDouble();
    bill["electricity"] = bill_electricity;

    bills.push_back(bill);
    bills_json_["bills"] = bills;
}

// 523.89
// 681.81
