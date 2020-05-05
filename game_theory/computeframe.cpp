#include <QDebug>
#include "computeframe.h"
#include "simplex.h"
#include "matrix.h"

ComputeFrame::ComputeFrame()
{
    QVBoxLayout *main_lay = new QVBoxLayout;
    main_lay->setSpacing(10);
    this->setWindowTitle("Cultures");

    int _default [8][10]= {
        { 3, 2, 1, 2, 4, 3, 1, 0, 0, 1},
        { 4, 3, 2, 2, 3, 4, 2, 1, 1, 0},
        { 3, 2, 1, 1, 2, 3, 4, 2, 3, 3},
        { 5, 4, 3, 3, 5, 5, 5, 3, 4, 4},
        { 6, 5, 4, 4, 4, 6, 5, 4, 3, 5},
        { 5, 6, 3, 3, 3, 5, 4, 3, 2, 3},
        { 3, 3, 2, 2, 4, 4, 4, 2, 4, 5},
        { 4, 4, 3, 1, 5, 3, 3, 2, 3, 4},
    };


    std::vector<double>_default_costs = {2, 3, 4, 3, 4, 5, 5, 6, 6, 8};

    QGridLayout *lay = new QGridLayout;
    lay->setSpacing(15);

    QLabel *weather = new QLabel("Weather");
    QLabel *cultures = new QLabel[10];
    lay->addWidget(weather,0,0);

    for(int i = 0;i < 10;i++)
    {
        cultures[i].setText(QString::number(i+1));
        lay->addWidget(&cultures[i],0,i+1);
    }

    for(int i = 0;i < 8;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            table[i][j].setValue(_default[i][j]);
            lay->addWidget(&table[i][j],i+1,j+1);
        }
    }


    QLabel *weathers = new QLabel[8];

    weathers[0].setText("(Cold,Drilly)");
    weathers[1].setText("(Cold,Normal)");
    weathers[2].setText("(Cold,Rainy)");
    weathers[3].setText("(Normal,Drilly)");
    weathers[4].setText("(Normal,Normal)");
    weathers[5].setText("(Normal,Rainy)");
    weathers[6].setText("(Hot,Drilly)");
    weathers[7].setText("(Hot,Normal)");

    for(int i = 0;i < 8;i++)
        lay->addWidget(&weathers[i],i+1,0);
    main_lay->addLayout(lay);
    QLabel *cost_lbl = new QLabel("Culture costs:");
    main_lay->addWidget(cost_lbl);

    QHBoxLayout *costs_lay = new QHBoxLayout;
    QLabel *cultures_2 = new QLabel[10];
    for(int i = 0;i < 10;i++) {
        cultures_2[i].setText(QString::number(i+1));
        costs_lay->addWidget(&cultures_2[i]);
    }
    main_lay->addLayout(costs_lay);
    QHBoxLayout *costs_spins = new QHBoxLayout;
    for(int i = 0;i < 10;i++) {
        costs[i].setValue(_default_costs[i]);
        costs_spins->addWidget(&costs[i]);
    }
    main_lay->addLayout(costs_spins);
    compute = new QPushButton("Compute");
    compute->setFixedSize(160,30);

    connect(compute,SIGNAL(clicked()),this,SLOT(compute_slot()));

    main_lay->addWidget(compute,Qt::AlignCenter);
    setLayout(main_lay);
}

void ComputeFrame::compute_slot()
{
    qDebug() << "compute";
    matrix<double>m(8,10);
    matrix<double>matre(8,10);
    std::vector<double>v;
    QList<double> result_p;
    QList<double> result_q;

    Simplex s(m);
    matre = s.Calculate(result_p);
    std::vector<double>_default_costs = {2, 3, 4, 3, 4, 5, 5, 6, 6, 8};

    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 10;j++)
            m(i,j) = table[i][j].value();

    for(int i = 0;i < 8;i++) {

        for(int j = 0;j < 10;j++) {

            m(i,j) = m(i,j) * _default_costs[j];
        }
    }
    // game_solve
    for(int i = 0;i < matrix.size1();i++)
        for(int j = 0;j < matrix.size2();j++)
            matrix(i,j) = matrix(i,j) * vector[j];

    matrix<double> m_extended(matrix.size1(),matrix.size2()+matrix.size1());
    for(int i = 0;i < matrix.size1();i++)
        for(int j = 0;j < matrix.size2();j++)
            m_extended(i,j) = matrix(i,j);

    int j;

    for(int i = matrix.size2(),j = 0;i < m_extended.size2();i++,j++)
    {
        f[i] = 0;
        m_extended(j,i) = -1;
    }

    QString str("p={");
    for(int i = 0;i < m_extended.size1();i++) {
        str.append(QString::fromStdString(std::to_string(result_p[i])));
        if(i!=m_extended.size1()-1)
            str.append(" ,");
    }

    matrix<double> tr_mat_extend(10,8);
    tr_mat_extend = matre.transponse();
    result_q = s.Calculate(tr_mat_extend);

    str.append("}\nq={");
    for(int i = 0;i < tr_mat_extend.size1();i++)
    {
        str.append(QString::fromStdString(std::to_string(result_q[i])));
        if(i!=tr_mat_extend.size1()-1) str.append(" ,");
    }
    str.append("}\n");


    QMessageBox msgBox;
    QSpacerItem* horizontalSpacer = new QSpacerItem(800, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    msgBox.setText(str);
    msgBox.setWindowTitle("Result");
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msgBox.exec();
}
