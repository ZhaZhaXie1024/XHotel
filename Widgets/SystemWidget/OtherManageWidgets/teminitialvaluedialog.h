#ifndef TEMINITIALVALUEDIALOG_H
#define TEMINITIALVALUEDIALOG_H

#include <QDialog>
#include <QVariant>

namespace Ui {
class TeminitialValueDialog;
}

class TeminitialValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeminitialValueDialog(QWidget *parent = nullptr);
    ~TeminitialValueDialog();

    //清除
    void clearInput();

public slots:
    //新增按钮被按下
    void insertBtnClicked();

private:
    Ui::TeminitialValueDialog *ui;

    QString m_title;

signals:
    void insertData(QVariantList values);
};

#endif // TEMINITIALVALUEDIALOG_H
