//
// Created by kira on 02.05.2021.
//

#include <QApplication>
#include "bills_window.hpp"

int main(int argc, char **argv) {
    QApplication app{argc, argv};
    BillsWindow bills_window{};
    bills_window.show();
    return QApplication::exec();
}
