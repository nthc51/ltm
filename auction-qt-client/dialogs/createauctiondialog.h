#ifndef CREATEAUCTIONDIALOG_H
#define CREATEAUCTIONDIALOG_H

#include <QDialog>

namespace Ui { class CreateAuctionDialog; }

class CreateAuctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAuctionDialog(QWidget *parent = nullptr);
    ~CreateAuctionDialog();
    
    QString getTitle() const;
    QString getDescription() const;
    double getStartPrice() const;
    double getBuyNowPrice() const;
    double getMinIncrement() const;
    int getDuration() const;

private:
    Ui::CreateAuctionDialog *ui;
};

#endif
