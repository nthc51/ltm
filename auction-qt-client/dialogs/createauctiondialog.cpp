#include "createauctiondialog.h"
#include "ui_createauctiondialog.h"

CreateAuctionDialog::CreateAuctionDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateAuctionDialog)
{
    ui->setupUi(this);
    setWindowTitle("Create Auction");
}

CreateAuctionDialog::~CreateAuctionDialog()
{
    delete ui;
}

QString CreateAuctionDialog::getTitle() const
{
    return ui->titleEdit->text();
}

QString CreateAuctionDialog::getDescription() const
{
    return ui->descEdit->toPlainText();
}

double CreateAuctionDialog::getStartPrice() const
{
    return ui->startPriceSpinBox->value();
}

double CreateAuctionDialog::getBuyNowPrice() const
{
    return ui->buyNowPriceSpinBox->value();
}

double CreateAuctionDialog::getMinIncrement() const
{
    return ui->minIncrementSpinBox->value();
}

int CreateAuctionDialog::getDuration() const
{
    return ui->durationSpinBox->value();
}
