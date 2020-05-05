#ifndef COMPUTEFRAME_H
#define COMPUTEFRAME_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "gamesolver.h"

class ComputeFrame : public QWidget
{
    Q_OBJECT

public:

    ComputeFrame();

private:
    QSpinBox table[8][10];
    QSpinBox costs[10];
    QPushButton *compute;

private slots:
    void compute_slot();
};
#endif // COMPUTEFRAME_H
