#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>

namespace Ui { class CreateRoomDialog; }

class CreateRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoomDialog(QWidget *parent = nullptr);
    ~CreateRoomDialog();
    
    QString getRoomName() const;
    QString getDescription() const;
    int getMaxParticipants() const;
    int getDuration() const;

private:
    Ui::CreateRoomDialog *ui;
};

#endif
