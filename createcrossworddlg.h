#ifndef CREATECROSSWORDDLG_H
#define CREATECROSSWORDDLG_H

#include <QDialog>
#include "BoardBuilder.h"

namespace Ui {
class CreateCrosswordDlg;
}

class CreateCrosswordDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCrosswordDlg(QWidget *parent = nullptr);
    ~CreateCrosswordDlg();

    std::shared_ptr<Board> CreateBoard();

    void SetWidth(unsigned int width);
    void SetHeight(unsigned int height);
    unsigned int GetWidth();
    unsigned int GetHeight();



private slots:

    void on_HeightSpinBox_valueChanged(int arg1);
    void on_WidthSpinBox_valueChanged(int arg1);
    void on_buttonBox_rejected();

private:
    unsigned int m_Width;
    unsigned int m_Height;

    bool isCanceled = false;
    Ui::CreateCrosswordDlg *ui;
};

#endif // CREATECROSSWORDDLG_H
