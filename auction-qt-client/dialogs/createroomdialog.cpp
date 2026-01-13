#include "createroomdialog.h"
#include "ui_createroomdialog.h"

CreateRoomDialog::CreateRoomDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateRoomDialog)
{
    ui->setupUi(this);
    setWindowTitle("Create Room");
}

CreateRoomDialog::~CreateRoomDialog()
{
    delete ui;
}

QString CreateRoomDialog::getRoomName() const
{
    return ui->nameEdit->text();
}

QString CreateRoomDialog::getDescription() const
{
    return ui->descEdit->toPlainText();
}

int CreateRoomDialog::getMaxParticipants() const
{
    return ui->maxParticipantsSpinBox->value();
}

int CreateRoomDialog::getDuration() const
{
    return ui->durationSpinBox->value();
}
