#include "matrix.h"
#include <QList>

class Simplex
{

public:
    matrix<double> table;


    int m, n;

    QList<int> basis;
    Simplex(matrix<double> source)
    {
        m = source.size1();
        n = source.size2();

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < table.size2(); j++)
            {
                if (j < n)
                    table(i, j) = source(i, j);
                else
                    table(i, j) = 0;
            }
            if ((n + i) < table.size2())
            {
                table(i, n + i) = 1;
                basis.append(n + i);
            }
        }

        n = table.size2();
    }

    matrix<double> Calculate(QList<double> &result)
    {
        int mainCol, mainRow;
        while (!IsItEnd())
        {
            mainCol = findMainCol();
            mainRow = findMainRow(mainCol);
            basis[mainRow] = mainCol;

            matrix<double> new_table(m,n);

            for (int j = 0; j < n; j++)
                new_table(mainRow, j) = table(mainRow, j) / table(mainRow, mainCol);

            for (int i = 0; i < m; i++)
            {
                if (i == mainRow)
                    continue;

                for (int j = 0; j < n; j++)
                    new_table(i, j) = table(i, j) - table(i, mainCol) * new_table(mainRow, j);
            }
            table = new_table;
        }

        for (int i = 0; i < result.size(); i++)
        {
            int k = basis.indexOf(i + 1);
            if (k != -1)
                result.insert(i,table(k, 0));
            else
                result.insert(i, 0);
        }
    }

    bool IsItEnd()
    {
        bool flag = true;

        for (int j = 1; j < n; j++)
        {
            if (table(m - 1, j) < 0)
            {
                flag = false;
                break;
            }
        }

        return flag;
    }

    int findMainCol()
    {
        int mainCol = 1;

        for (int j = 2; j < n; j++)
            if (table(m - 1, j) < table(m - 1, mainCol))
                mainCol = j;

        return mainCol;
    }

    int findMainRow(int mainCol)
    {
        int mainRow = 0;

        for (int i = 0; i < m - 1; i++)
            if (table(i, mainCol) > 0)
            {
                mainRow = i;
                break;
            }

        for (int i = mainRow + 1; i < m - 1; i++)
            if ((table(i, mainCol) > 0) && ((table(i, 0) / table(i, mainCol)) < (table(mainRow, 0) / table(mainRow, mainCol))))
                mainRow = i;

        return mainRow;
    }


};
