#pragma once

#include <QtWidgets/QWidget>
#include "ui_VisGraph.h"

class VisGraph : public QWidget
{
    Q_OBJECT

public:
    VisGraph(QWidget *parent = nullptr);
    ~VisGraph();

private:
    Ui::VisGraphClass ui;
};
