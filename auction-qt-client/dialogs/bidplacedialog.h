#ifndef BIDPLACEDIALOG_H
#define BIDPLACEDIALOG_H

#include <QDialog>
#include "../models/auction.h"

class QDoubleSpinBox;
class QLabel;
class QPushButton;

class BidPlaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BidPlaceDialog(const Auction& auction, QWidget *parent = nullptr);
    double getBidAmount() const;

private slots:
    void validateBidAmount(double value);
    void hideWarning();
    void onOkClicked();

private:
    Auction auction;
    QDoubleSpinBox *bidSpinBox;
    QLabel *warningLabel;
    QPushButton *okButton;
};

#endif // BIDPLACEDIALOG_H