#include "bidplacedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <cmath>
#include "../utils/formatters.h"

BidPlaceDialog::BidPlaceDialog(const Auction& auction, QWidget *parent)
    : QDialog(parent), auction(auction)
{
    setWindowTitle("💰 Đặt Giá - " + auction.title);
    setMinimumWidth(450);
    setModal(true);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    
    // ==================== TITLE ====================
    QLabel *titleLabel = new QLabel("<h2>💎 " + auction.title + "</h2>");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #1976d2; padding: 10px;");
    layout->addWidget(titleLabel);
    
    // ==================== INFO GROUP ====================
    QWidget *infoWidget = new QWidget();
    infoWidget->setStyleSheet(
        "QWidget { background: #f5f5f5; border-radius: 10px; padding: 15px; }"
    );
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    
    // Current price
    QLabel *currentPriceLabel = new QLabel(QString(
        "💰 Giá hiện tại: <b style='color: #4caf50; font-size: 16px;'>%1</b>"
    ).arg(Formatters::formatCurrency(auction.currentPrice)));
    currentPriceLabel->setStyleSheet("font-size: 14px; padding: 5px;");
    infoLayout->addWidget(currentPriceLabel);
    
    // Min bid with step
    double minBid = auction.currentPrice + auction.minIncrement;
    QLabel *minBidLabel = new QLabel(QString(
        "📊 Giá tối thiểu: <b style='color: #f57c00;'>%1</b> "
        "(bước nhảy: <b>%2</b>)"
    ).arg(Formatters::formatCurrency(minBid))
     .arg(Formatters::formatCurrency(auction.minIncrement)));
    minBidLabel->setStyleSheet("font-size: 13px; padding: 5px;");
    infoLayout->addWidget(minBidLabel);
    
    layout->addWidget(infoWidget);
    
    // ==================== BID INPUT ====================
    QWidget *bidWidget = new QWidget();
    bidWidget->setStyleSheet(
        "QWidget { background: white; border: 2px solid #e0e0e0; "
        "border-radius: 10px; padding: 15px; }"
    );
    QVBoxLayout *bidLayout = new QVBoxLayout(bidWidget);
    
    QLabel *bidInputLabel = new QLabel("💵 Nhập giá đặt của bạn:");
    bidInputLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    bidLayout->addWidget(bidInputLabel);
    
    bidSpinBox = new QDoubleSpinBox();
    bidSpinBox->setRange(minBid, 999999999.0);
    bidSpinBox->setValue(minBid);
    bidSpinBox->setSingleStep(auction.minIncrement);
    bidSpinBox->setDecimals(0);
    bidSpinBox->setSuffix(" VND");
    bidSpinBox->setStyleSheet(
        "QDoubleSpinBox { padding: 12px; font-size: 16px; font-weight: bold; "
        "border: 2px solid #4caf50; border-radius: 8px; }"
    );
    bidLayout->addWidget(bidSpinBox);
    
    // Info label
    QLabel *infoLabel = new QLabel(
        "ℹ️ <i>Giá sẽ tự động điều chỉnh theo bội của bước nhảy</i>"
    );
    infoLabel->setStyleSheet("color: #666; font-size: 12px; padding: 5px;");
    infoLabel->setWordWrap(true);
    bidLayout->addWidget(infoLabel);
    
    // Warning label (hidden by default)
    warningLabel = new QLabel();
    warningLabel->setStyleSheet(
        "color: #f57c00; font-weight: bold; font-size: 13px; "
        "background: #fff3e0; padding: 8px; border-radius: 5px;"
    );
    warningLabel->setVisible(false);
    warningLabel->setWordWrap(true);
    bidLayout->addWidget(warningLabel);
    
    layout->addWidget(bidWidget);
    
    // ==================== VALIDATOR ====================
    connect(bidSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BidPlaceDialog::validateBidAmount);
    
    // ==================== BUTTONS ====================
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    okButton = new QPushButton("✅ Đặt giá");
    okButton->setStyleSheet(
        "QPushButton { background: #4caf50; color: white; padding: 12px 30px; "
        "font-size: 15px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #45a049; } "
        "QPushButton:disabled { background: #ccc; }"
    );
    
    QPushButton *cancelButton = new QPushButton("❌ Hủy");
    cancelButton->setStyleSheet(
        "QPushButton { background: #f44336; color: white; padding: 12px 30px; "
        "font-size: 15px; font-weight: bold; border-radius: 8px; } "
        "QPushButton:hover { background: #da190b; }"
    );
    
    connect(okButton, &QPushButton::clicked, this, &BidPlaceDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
}

void BidPlaceDialog::validateBidAmount(double value)
{
    double minRequired = auction.currentPrice + auction.minIncrement;
    
    // Check if below minimum
    if (value < minRequired) {
        warningLabel->setText(QString(
            "⚠️ Giá quá thấp! Tối thiểu: %1"
        ).arg(Formatters::formatCurrency(minRequired)));
        warningLabel->setVisible(true);
        okButton->setEnabled(false);
        return;
    }
    
    // Check if valid multiple of minIncrement
    double diff = value - auction.currentPrice;
    double remainder = fmod(diff, auction.minIncrement);
    
    if (remainder > 0.01) {  // Not a valid multiple (tolerance for floating point)
        // Calculate correct rounded value
        double rounded = round(diff / auction.minIncrement) * auction.minIncrement;
        double newValue = auction.currentPrice + rounded;
        
        // Show warning
        warningLabel->setText(QString(
            "⚠️ Đã điều chỉnh từ %1 → %2"
        ).arg(Formatters::formatCurrency(value))
         .arg(Formatters::formatCurrency(newValue)));
        warningLabel->setVisible(true);
        
        // Snap to valid value
        bidSpinBox->blockSignals(true);
        bidSpinBox->setValue(newValue);
        bidSpinBox->blockSignals(false);
        
        okButton->setEnabled(true);
        
        // Hide warning after 3 seconds
        QTimer::singleShot(3000, this, &BidPlaceDialog::hideWarning);
    } else {
        // Valid bid
        warningLabel->setVisible(false);
        okButton->setEnabled(true);
    }
}

void BidPlaceDialog::hideWarning()
{
    warningLabel->setVisible(false);
}

void BidPlaceDialog::onOkClicked()
{
    double bidAmount = bidSpinBox->value();
    double minRequired = auction.currentPrice + auction.minIncrement;
    
    // Final validation before accepting
    if (bidAmount < minRequired) {
        QMessageBox::warning(this, "Lỗi", 
            QString("Giá đặt phải ít nhất %1")
            .arg(Formatters::formatCurrency(minRequired)));
        return;
    }
    
    // Check valid multiple
    double diff = bidAmount - auction.currentPrice;
    double remainder = fmod(diff, auction.minIncrement);
    
    if (remainder > 0.01) {
        QMessageBox::warning(this, "Lỗi", 
            QString("Giá phải là bội của %1")
            .arg(Formatters::formatCurrency(auction.minIncrement)));
        return;
    }
    
    accept();
}

double BidPlaceDialog::getBidAmount() const
{
    return bidSpinBox->value();
}