#include "algorithmtab.h"
#include "../../MainWindow.h"
#include "../dialogs/AlgorithmParametersDialog.h"
#include "ResultsTabWidget.h"
#include "QHeaderView"
#include <QEventLoop>
#include <QDebug>

AlgorithmTab::AlgorithmTab(QWidget * parent, std::string uuid) : QWidget(parent)
{
        m_parent = parent;
        m_uuid = uuid;

        //By default
        selectedIndexRow = 0;

        getAlgorithmsFromDB();

        // -- Layout
        algorithmListGroupBox = new QGroupBox();
        algorithmListGroupBox->setFixedHeight(300);
        algorithmListGroupBox->setFlat(true);
        algorithmListLayout = new QVBoxLayout();

        parametersGroupBox = new QGroupBox();
        parametersGroupBox->setFixedHeight(250);
        parametersGroupBox->setFlat(true);
        parametersLayout = new QVBoxLayout();

        algorithmTabLayout = new QVBoxLayout();


        // -- Algorithm List Section
        algorithmLabel = new QLabel(tr("Tableau des algorithmes disponibles"));
        algorithmTableWidget = new QTableWidget(this);

        algorithmTableWidget->setRowCount(10);
        algorithmTableWidget->setColumnCount(3);

        algorithmTableHeaders<<"Nom"<<"Description"<<"Auteur";

        algorithmTableWidget->setHorizontalHeaderLabels(algorithmTableHeaders);

        QHeaderView * headerHoriz = algorithmTableWidget->horizontalHeader();
        QHeaderView * headerVerti = algorithmTableWidget->verticalHeader();

        headerHoriz->setSectionResizeMode(QHeaderView::Stretch);
        headerVerti->setSectionResizeMode(QHeaderView::Stretch);

        algorithmTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        algorithmTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        algorithmTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

        for(int i =0; i<algoList.m_algorithmList.size();i++)
        {
            QString name = QString::fromStdString(algoList.m_algorithmList.at(i).name);
            algorithmTableWidget->setItem(i, 0, new QTableWidgetItem(name));

            QString description = QString::fromStdString(algoList.m_algorithmList.at(i).description);
            algorithmTableWidget->setItem(i, 1, new QTableWidgetItem(description));

            QString author = QString::fromStdString(algoList.m_algorithmList.at(i).author);
            algorithmTableWidget->setItem(i, 2, new QTableWidgetItem(author));
        }
        algorithmTableWidget->setRowCount(algoList.m_algorithmList.size());

        connect(algorithmTableWidget, SIGNAL(doubleClicked(const QModelIndex& )), this, SLOT(openParametersWindow(const QModelIndex &)));

        // -- Parameter Section
        currentSelectionLabel = new QLabel(tr("Sélection courante"));
        selectedDataLabel = new QLabel(tr("Données sélectionnées: "));
        selectedDataValues = new QLabel(tr(" "));
        selectedAlgorithmLabel = new QLabel(tr("Algorithme sélectionné: "));
        selectedAlgorithmValues = new QLabel(tr(" "));
        parametersLabel = new QLabel(tr("Paramètre(s):"));
        parametersValues = new QLabel(tr(""));
        parametersValues->setWordWrap(true);

        applyAlgorithm = new QPushButton(tr("Appliquer algorithme"));
        connect(applyAlgorithm, SIGNAL(clicked()),this, SLOT(openResultTab()));

        // -- Setting the layout
        algorithmListLayout->addWidget(algorithmLabel);
        algorithmListLayout->addWidget(algorithmTableWidget);
        algorithmListGroupBox->setLayout(algorithmListLayout);

        parametersLayout->addWidget(currentSelectionLabel);
        parametersLayout->addWidget(selectedDataLabel);
        parametersLayout->addWidget(selectedDataValues);
        parametersLayout->addWidget(selectedAlgorithmLabel);
        parametersLayout->addWidget(selectedAlgorithmValues);
        parametersLayout->addWidget(parametersLabel);
        parametersLayout->addWidget(parametersValues);
        parametersGroupBox->setLayout(parametersLayout);

        algorithmTabLayout->addWidget(algorithmListGroupBox);
        algorithmTabLayout->addSpacing(50);
        algorithmTabLayout->addWidget(parametersGroupBox);
        algorithmTabLayout->addWidget(applyAlgorithm);

        this->setLayout(algorithmTabLayout);
        this->setStyleSheet( "QPushButton{"
                             "background-color: rgba(119, 160, 175,0.7);"
                             "border-style: inset;"
                             "border-width: 0px;"
                             "border-radius: 10px;"
                             "border-color: white;"
                             "font: 12px;"
                             "min-width: 10em;"
                             "padding: 6px; }"
                             "QPushButton:pressed { background-color: rgba(70, 95, 104, 0.7);}"
                             );
}

void AlgorithmTab::resetSelectionSection()
{
    selectedDataValues->setText("");
    selectedAlgorithmValues->setText("");
    parametersValues->setText("");
}

void AlgorithmTab::setAlgorithm(AlgorithmInfo algorithmInfo)
{
    resetSelectionSection();
    selectedAlgorithm = algoList.m_algorithmList.at(selectedIndexRow);
    selectedAlgorithm.parameters.swap(algorithmInfo.parameters);

    if(algorithmInfo.parameters.size() == 0 ||
            ((algorithmInfo.parameters.size() == 1) && (algorithmInfo.parameters.at(0).name == "uuid")))
    {
        parametersValues->setText("Aucun paramètre pour cet algorithme");
    }
    else
    {
        for(int i=0; i<algorithmInfo.parameters.size();i++)
        {
            if(selectedAlgorithm.parameters.at(i).name != "uuid")
            {
                QString parameterName = QString::fromStdString(selectedAlgorithm.parameters.at(i).name);
                QString parameterValue = QString::fromStdString(selectedAlgorithm.parameters.at(i).value);

                QString previousParameters = parametersValues->text();
                parametersValues->setText(previousParameters + parameterName + ": " + parameterValue+ "\n" );
            }
        }
    }
    selectedAlgorithmValues->setText(QString::fromStdString(selectedAlgorithm.name));
}


void AlgorithmTab::openResultTab()
{
    createAlgoRequest();
}

void AlgorithmTab::openParametersWindow(const QModelIndex &index)
{
    if (index.isValid() && algoList.m_algorithmList.size() != 0)
    {
        //Retrieve the selected Algorithm and it's parameters
        AlgorithmInfo clickedAlgorithm = algoList.m_algorithmList.at(index.row());
        selectedIndexRow = index.row();

        if((clickedAlgorithm.parameters.size() <= 0)||
                ((clickedAlgorithm.parameters.size() == 1) && (clickedAlgorithm.parameters.at(0).name == "uuid")))
        {
            setAlgorithm(clickedAlgorithm);
        }
        else
        {
            AlgorithmParametersDialog * algorithmParametersWindow = new AlgorithmParametersDialog(this, clickedAlgorithm);
            algorithmParametersWindow->exec();
            delete algorithmParametersWindow;
        }
    }
}

bool AlgorithmTab::createAlgoRequest()
{
    std::string algoName = selectedAlgorithm.name;
    std::string url = "http://127.0.0.1:5000/algo?filename="+algoName+"&uuid="+m_uuid;

    for(int i=0; i< selectedAlgorithm.parameters.size();i++)
    {
        if(selectedAlgorithm.parameters.at(i).name != "uuid")
        {
            url = url + "&"+selectedAlgorithm.parameters.at(i).name +"="+selectedAlgorithm.parameters.at(i).value;
        }
    }

    QNetworkRequest request(QUrl(QString::fromStdString(url)));
    request.setRawHeader("User-Agent", "ApplicationNameV01");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    bool result = connect(manager, SIGNAL(finished(QNetworkReply*)), &loop,SLOT(quit()));
    loop.exec();
    reponseAlgoRecue(reply);

    return true;
}

bool AlgorithmTab::getAlgorithmsFromDB()
{
    QNetworkRequest request(QUrl("http://127.0.0.1:5000/algolist"));
    request.setRawHeader("User-Agent", "ApplicationNameV01");
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    bool result = connect(manager, SIGNAL(finished(QNetworkReply*)), &loop,SLOT(quit()));
    loop.exec();
    reponseRecue(reply);

    return true;
}

void AlgorithmTab::reponseRecue(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
   {
       std::string testReponse = reply->readAll();
       CJsonSerializer::Deserialize(&algoList, testReponse);
   }
   else
   {
       //qDebug() << "error connect";
       //qWarning() <<"ErrorNo: "<< reply->error() << "for url: " << reply->url().toString();
       //qDebug() << "Request failed, " << reply->errorString();
       //qDebug() << "Headers:"<<  reply->rawHeaderList()<< "content:" << reply->readAll();
       //qDebug() << reply->readAll();
   }
   delete reply;
}

void AlgorithmTab::reponseAlgoRecue(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
   {
       std::string reponse = reply->readAll();
       //qDebug() << QString::fromStdString(reponse);

       AlgorithmOutput output;
       CJsonSerializer::Deserialize(&output, reponse);
       MainWindow * test = (MainWindow*)m_parent;
       ResultsTabWidget* res = new ResultsTabWidget(this, algoList.m_algorithmList.at(selectedIndexRow), output);
       test->replaceTab(res,"Résultats");
   }
   else
   {
       //qDebug() << "Error connect";
       //qWarning() <<"ErrorNo: "<< reply->error() << "for url: " << reply->url().toString();
       //qDebug() << "Request failed, " << reply->errorString();
       //qDebug() << "Headers:"<<  reply->rawHeaderList()<< "content:" << reply->readAll();
       //qDebug() << reply->readAll();
   }
   delete reply;
}
