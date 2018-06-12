#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    signalMapper = new QSignalMapper(this);

    rowCount=0;

    plcok = false;
    conplc = new Plcqtlib();
    text = new QLabel();
    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(TimEvent()));


    progressBar = new QProgressBar();
    progressBar->setRange(0,5);
    progressBar->setTextVisible(false);
    progressBar->setMinimumSize(20,2);


    connect(ui->conButton, SIGNAL(clicked()),this, SLOT(ConnectPlc()));
    //   connect(this, SIGNAL(NetPlcStato(bool)),ui->radioCon, SLOT(setChecked(bool)));
    //connect(ui->helpButton, SIGNAL(clicked()),this, SLOT(Help()));

    //    connect(ui->runButton, SIGNAL(clicked()),this, SLOT(RunPlc()));
    //    connect(ui->stopButton, SIGNAL(clicked()),this, SLOT(StopPlc()));
    //    connect(ui->readButton, SIGNAL(clicked()),this, SLOT(ReadDb()));
    //    connect(ui->writeDb, SIGNAL(clicked()),this, SLOT(WriteDb()));
    connect(this,SIGNAL(NetPlcStato(bool)),this,SLOT(PlcOk(bool)));
    //    connect(ui->setByte, SIGNAL(clicked()),this, SLOT(SetByte()));
    //    connect(ui->setBit, SIGNAL(clicked()),this, SLOT(SetBit()));
    //    connect(ui->resetBit, SIGNAL(clicked()),this, SLOT(ResetBit()));

    //    QList<QHostAddress> list;
    //    qnetint=new QNetworkInterface();
    //    list=qnetint->allAddresses();
    //    QString str;
    //    for (int i = 0; i < list.size(); ++i) {
    //        str = list.at(i).toString();
    //        ui->ListIp->addItem(str);
    //    }

    //  customPlot = ui->customPlot;

    //    demoName = "Quadratic Demo";
    //    customPlot->addGraph();
    //    // give the axes some labels:
    //    customPlot->xAxis->setLabel("x");
    //    customPlot->yAxis->setLabel("y");
    //    // set axes ranges, so we see all data:
    //    customPlot->xAxis->setRange(0,50);
    //    customPlot->yAxis->setRange(0, 200);
    //    ui->customPlot->axisRect()->setupFullAxesBox(true);
    //    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
    //                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    //        ui->customPlot->legend->setVisible(true);



    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setMinimumWidth(30);

    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(24);

    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    ui->tableWidget->setColumnWidth(0,120);
    ui->tableWidget->setColumnWidth(1,60);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,120);
    ui->tableWidget->setColumnWidth(4,40);

    QAction* actBool = tableContMenuType.addAction("BOOL");
    QAction* actByte = tableContMenuType.addAction("BYTE");
    QAction* actUbyte =  tableContMenuType.addAction("UBYTE");
    QAction* actInt =  tableContMenuType.addAction("INT");
    QAction* actDint =  tableContMenuType.addAction("DINT");
    QAction* actWord =  tableContMenuType.addAction("WORD");
    QAction* actDword =  tableContMenuType.addAction("DWORD");
    QAction* actFloat =   tableContMenuType.addAction("FLOAT");

    connect(actBool, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actByte, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actUbyte, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actInt, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actDint, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actWord, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actDword, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(actFloat, SIGNAL(triggered()), signalMapper, SLOT(map()));

    signalMapper->setMapping(actBool, actBool->text());
    signalMapper->setMapping(actByte, actByte->text());
    signalMapper->setMapping(actUbyte, actUbyte->text());
    signalMapper->setMapping(actInt, actInt->text());
    signalMapper->setMapping(actDint, actDint->text());
    signalMapper->setMapping(actWord, actWord->text());
    signalMapper->setMapping(actDword, actDword->text());
    signalMapper->setMapping(actFloat, actFloat->text());

    connect(signalMapper, SIGNAL(mapped(const QString &)),this, SLOT(tableContextMenuTypeClick(const QString &)));
    connect(ui->tableWidget->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotSectionClicked(int)));
    plotcount=0;

    //    itm[rowCount++] = new Item(ui->tableWidget,rowCount);
    //    thr = new MyThread();
    //    //connect(thr, SIGNAL(objectIsReady()), this, SLOT(Slot()));
    //    thr->start();

    mPlotWindow = new PlotWindow();
    customPlot = mPlotWindow->customPlot;

    customPlot->addGraph(); // blue line
    //    customPlot->graph(0)->setPen(QPen(Qt::blue));
    //    customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    //    customPlot->graph(0)->setAntialiasedFill(false);

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();

    customPlot->axisRect()->setupFullAxesBox(true);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    customPlot->legend->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Add custom row to table widget
void MainWindow::on_pbAddRow_clicked()
{
    itm[rowCount] = new Item(ui->tableWidget,rowCount);
    itm[rowCount]->Init();
    customPlot->addGraph();
    itm[rowCount]->graph = customPlot->graph(rowCount);
    timer->start(10);

    rowCount++;
    qDebug() << "Add row";
}

//TableWidget context menu request
void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<" Clicked cm on (" << ui->tableWidget->rowAt(pos.y())<<","<< ui->tableWidget->columnAt(pos.x())<<")";

    //Show context menu for column = 1 ("Type")
    if (ui->tableWidget->columnAt(pos.x())==1){
        tableContMenuType.popup(ui->tableWidget->viewport()->mapToGlobal(pos));
        qDebug()<<" Show cm Type";
    }
}


//Click on TableWidget Type context menu
void MainWindow::tableContextMenuTypeClick(const QString & text)
{
    int x=ui->tableWidget->currentRow() ;
    itm[x]->setType(text);
    qDebug()<<"Type cm select -> " << text;
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (ui->tableWidget->currentColumn()==0){
        itm[row]->ParseAddressStr();
        qDebug()<<" Changed cell";

    }
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return )
        qDebug()<<" event->text()";

}




void MainWindow::slotSectionClicked(int){
    qDebug()<<" Activate row";
}


void MainWindow::TimEvent()
{

    for(int i=0;i<rowCount;i++){

        if (itm[i]->getEn()){
            break;
        }
        if (i == rowCount-1){
            return;
        }
    }
    if(plcok == true){
        float datoint ;
        datoint = conplc ->getDbReal(1,2);
    }

    mPlotWindow->UpdateTimeValue->setDate(QDate::currentDate());
    mPlotWindow->VisiblePeriodValue->setDate(QDate::currentDate());
    QDateTime tOnlyDate;
    tOnlyDate.setDate(QDate::currentDate());
    double updPeriod = (mPlotWindow->UpdateTimeValue->dateTime().toMSecsSinceEpoch() - tOnlyDate.toMSecsSinceEpoch())/1000.0;
    double visiblePeriod = (mPlotWindow->VisiblePeriodValue->dateTime().toMSecsSinceEpoch()- tOnlyDate.toMSecsSinceEpoch()) /1000.0;

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;

    static double lastPointKey = 0;


    static double v[10]={0.0};

    if (key-lastPointKey > updPeriod) // at most add point every 10 ms
    {

        for(int i=0;i<rowCount;i++){
            v[i]+=10.0;

            if(itm[i]->getEn()){
                customPlot->graph(i)->addData(key, v[i]);
                itm[i]->setValue(QString::number(v[i]));
            }
            if (v[i]>200.0)
                v[i]=0.0;
        }
        // remove data of lines that's outside visible range:
        customPlot->graph(0)->removeDataBefore(key-visiblePeriod);
        // rescale value (vertical) axis to fit the current data:
        //customPlot->graph(0)->rescaleValueAxis();
        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    customPlot->xAxis->setRange(key+0.25, visiblePeriod, Qt::AlignRight);
    customPlot->replot();

}

void MainWindow::ConnectPlc()
{

    statotcp = conplc->setTcp(ui->ipEdit->text(),102);

    if (ui->comboPlc->currentText() == "S7300" ){
        mpi = 2;
        rack = 0;
        slot = 2;

    }else if(ui->comboPlc->currentText() == "S7200"){
        mpi = 2;
        rack = 0;
        slot = 0;

    }else if(ui->comboPlc->currentText() == "LOGO"){
        mpi = 1;
        rack = 1;
        slot = 0;

    }else if(ui->comboPlc->currentText() == "S71200"){
        mpi = 2;
        rack = 0;
        slot = 0;
    }

    int statoplc = conplc->setPlc(ui->comboPlc->currentText(),mpi,rack,slot);
    if ((statotcp == 0) & (statoplc == 0) ){
        stato = true;
        emit NetPlcStato(stato);

        QString adr;

        //  ui->ListDebug->addItem(conplc->getAddress());
        int port;
        port=conplc->getPort();
        adr = "port:" + adr.setNum(port);
        //  ui->ListDebug->addItem(adr);
        int sock;
        sock=conplc->getSocketDescriptor();
        adr = "socket:" + adr.setNum(sock);
        //  ui->ListDebug->addItem(adr);
        adr=conplc->getError();
        //  ui->ListDebug->addItem(adr);
        adr = adr.setNum(statoplc);
        //   ui->ListDebug->addItem(adr);


    }else {
        stato = false;
        this->Mess();
    }

}


void MainWindow::PlcOk(bool ok)
{
    plcok = ok;

    if(plcok == true)
        ui->statusBar->addWidget(progressBar,0);
    text->setText("On Line");
    ui->statusBar->addWidget(text,1);

}


void MainWindow::Mess()
{
    QMessageBox msgBox;
    msgBox.setText("Not connected Plc");
    msgBox.exec();

}

void MainWindow::Slot()
{
    // ui->customPlot->replot();

}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<<" Clicked";
}

void MainWindow::on_addValue_clicked()
{
    itm[0]->Value.setText("100");
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionShow_triggered()
{
    mPlotWindow->show();
}

