#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kfTableModel.h"


#include "pugixml.hpp"
#include "ResvgQt.h"

#include "SVGObject.hpp"

#include<QInputDialog>
#include<QFileDialog>
#include<QMessageBox>
#include<QStringListModel>
#include<QDebug>
#include <QColorDialog>


int counter=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Setup SVG hiraerchy view UI*/
    auto stringListModel = new QStringListModel(this);
    QStringList stringListSVG;
    stringListSVG << "Some Item";
    stringListModel->setStringList(stringListSVG);
    ui->listView->setModel(stringListModel);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /*Setup Keyframe data view UI*/
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList{"Keyframe Number", "Object ID", "Property Name", "Value"});
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    qInfo() << "C++ Style Info Message";
    ui->textEdit->setText(currentFile);
}


void MainWindow::on_actionSave_triggered()
{
    currentFile=ui->textEdit->toPlainText();
}

void MainWindow::textChanged(){
    counter+=1;
    if(counter%100==0){
        QMessageBox warning;
        warning.setText("You have changed the text");
        warning.exec();
    }
}*/


void MainWindow::on_actionOpen_SVG_triggered()
{
    qInfo() << "Opening SVG";
    svgOpened=true;
    mainSVGObject=new NatronSVGObject();
    mainSVGObject->doc=new pugi::xml_document();
    QString filePath=QFileDialog::getOpenFileName(this,tr("Open SVG file"), "/home", tr("SVG Files (*.svg)"));
    pugi::xml_parse_result res=mainSVGObject->doc->load_file(filePath.toLocal8Bit());
    ResvgOptions* opt= new ResvgOptions();
    ResvgRenderer* x= new ResvgRenderer();
    if(res.status==pugi::xml_parse_status::status_ok){
        pugi::xpath_node_set tempNodeSet=mainSVGObject->GetObjectsList();
        auto model = new QStringListModel(this);
        QStringList list;
        for(auto it=tempNodeSet.begin();it!=tempNodeSet.end();++it){
            list << it->node().attribute("id").value();
        }
        model->setStringList(list);
        ui->listView->setModel(model);
        mainSVGObject->keyframeData=new pugi::xml_document();
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Error opening SVG file");
        msgBox.setInformativeText(res.description());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
    }

}

/*void MainWindow::on_actionSave_Keyframe_File_triggered(){

}*/
void MainWindow::on_actionOpen_Keyframe_File_triggered()
{
    if(!svgOpened){
        QMessageBox::warning(this, tr("VecFX"),
                                       tr("You haven't opened an SVG. Open one first"),
                                       QMessageBox::Ok);
    }
    else{
        qInfo() << "Kf file opened";

        mainSVGObject->keyframeData=new pugi::xml_document();
        mainSVGObject->keyframeData->load_file(QFileDialog::getOpenFileName(this,tr("Open XML File 1"), "/home", tr("XML Files (*.xml)")).toLocal8Bit());
        ui->tableWidget->blockSignals(true);
        for(auto kfIt=mainSVGObject->keyframeData->first_child();kfIt;kfIt=kfIt.next_sibling()){
            for(auto objectIt=kfIt.first_child();objectIt;objectIt=objectIt.next_sibling()){
                for(auto propertyIt=objectIt.first_attribute();propertyIt;propertyIt=propertyIt.next_attribute()){
                    auto rowPos=ui->tableWidget->rowCount();
                    ui->tableWidget->insertRow(rowPos);
                    auto itemKfNum=new QTableWidgetItem();
                    qInfo() << "The keyframe number is: " << QString(kfIt.name());
                    itemKfNum->setText(QString(kfIt.name()).remove(0,1));
                    auto itemObjectId=new QTableWidgetItem();
                    itemObjectId->setText(QString(objectIt.name()));
                    auto itemPropertyName=new QTableWidgetItem(QString(propertyIt.name()));
                    itemPropertyName->setText(QString(propertyIt.name()));
                    auto itemValue=new QTableWidgetItem(QString(propertyIt.value()));
                    itemValue->setText(QString(propertyIt.value()));
                    qInfo() << "Keyframe number:" << kfIt.name() << "\n";
                    qInfo() << "Object ID:" << objectIt.name() << "\n";
                    qInfo() << "Property name:" << propertyIt.name() << "\n";
                    qInfo() << "Value:" << propertyIt.value() << "\n";
                    ui->tableWidget->setItem(rowPos, 0, itemKfNum);
                    ui->tableWidget->setItem(rowPos, 1, itemObjectId);
                    ui->tableWidget->setItem(rowPos, 2, itemPropertyName);
                    ui->tableWidget->setItem(rowPos, 3, itemValue);
                }
            }
        }
        ui->tableWidget->blockSignals(false);
    }
}


void MainWindow::cellChangedTable(int i, int j){
    qInfo() << "Reached here";
    ui->tableWidget->blockSignals(true);
    mainSVGObject->keyframeData=new pugi::xml_document();
    for(int rowCounter=0;rowCounter!=ui->tableWidget->rowCount();rowCounter++){
        std::string objectId,value, attributeName;
        unsigned int kfNum;
        objectId=ui->tableWidget->item(rowCounter,1)->text().toLocal8Bit().constData();
        attributeName=ui->tableWidget->item( rowCounter,2)->text().toLocal8Bit().constData();
        value=ui->tableWidget->item(rowCounter,3)->text().toLocal8Bit().constData();
        kfNum=ui->tableWidget->item( rowCounter,0)->text().toUInt();
        qInfo() << "Keyframe number:" << kfNum << "\n";
        qInfo() << "Object ID:" << objectId.c_str() << "\n";
        qInfo() << "Property name:" << attributeName.c_str() << "\n";
        qInfo() << "Value:" << value.c_str() << "\n";
        qInfo() << "Changing XML \n";
        mainSVGObject->SetObjectAttributeAtKeyframe(objectId, attributeName, value, kfNum);
    }
    qInfo() << "Document:\n";
    mainSVGObject->keyframeData->save_file("test.xml");
    ui->tableWidget->blockSignals(false);
}

void MainWindow::rowAdded(){
    if(svgOpened){
        bool ok;
        int kfNum = QInputDialog::getInt(this, tr("Added row: Keyframe"),tr("Keyframe number:"), QLineEdit::Normal,0, 3200, 1, &ok);
        if(ok){
            QStringList possibleObjectIds;
            pugi::xpath_node_set tempNodeSet=mainSVGObject->GetObjectsList();
            for(auto it=tempNodeSet.begin();it!=tempNodeSet.end();++it){
                possibleObjectIds << it->node().attribute("id").value();
            }
            qInfo() << "Reached here";
            QString objectId = QInputDialog::getItem(this, tr("Added row: Object ID"),
                                                                   tr("Object Id:"), possibleObjectIds, 0, false, &ok);;
            if(ok && !objectId.isEmpty()){
                QStringList possibleAttributesList;
                for(auto it=possibleAttributes.begin();it!=possibleAttributes.end();++it){
                    possibleAttributesList << it->first.c_str();
                }
                QString attribute = QInputDialog::getItem(this, tr("Added row: Property Name"),
                                                                       tr("Attribute:"), possibleAttributesList, 0, false, &ok);
                if(ok){
                    int valueType;

                    for(auto it=possibleAttributes.begin();it!=possibleAttributes.end();++it){
                        if(attribute==it->first.c_str()){
                            valueType=it->second;
                        }
                    }
                    if(valueType==1){
                        double value = QInputDialog::getDouble(this, tr("Added row: Value"),tr("Value:"), QLineEdit::Normal,0, 3200, 1, &ok);
                        mainSVGObject->SetObjectAttributeAtKeyframe(objectId.toLocal8Bit().constData(), attribute.toLocal8Bit().constData(), std::to_string(value), kfNum);
                        ui->tableWidget->blockSignals(true);
                        /*TODO -> Explain Block signals*/
                        auto rowPos=ui->tableWidget->rowCount();
                        ui->tableWidget->insertRow(rowPos);
                        auto itemKfNum=new QTableWidgetItem();
                        itemKfNum->setText(QString::number(kfNum));
                        auto itemObjectId=new QTableWidgetItem();
                        itemObjectId->setText(objectId);
                        auto itemPropertyName=new QTableWidgetItem();
                        itemPropertyName->setText(attribute);
                        auto itemValue=new QTableWidgetItem();
                        itemValue->setText(QString::number(value));
                        ui->tableWidget->setItem(rowPos,0,itemKfNum);
                        ui->tableWidget->setItem(rowPos,1,itemObjectId);
                        ui->tableWidget->setItem(rowPos,2,itemPropertyName);
                        ui->tableWidget->setItem(rowPos,3,itemValue);
                        ui->tableWidget->blockSignals(false);
                    }
                    else if(valueType==2){
                        auto value=QColorDialog::getColor().name();
                        mainSVGObject->SetObjectAttributeAtKeyframe(objectId.toLocal8Bit().constData(), attribute.toLocal8Bit().constData(), value.toStdString(), kfNum);
                        ui->tableWidget->blockSignals(true);
                        auto rowPos=ui->tableWidget->rowCount();
                        ui->tableWidget->insertRow(rowPos);
                        auto itemKfNum=new QTableWidgetItem();
                        itemKfNum->setText(QString::number(kfNum));
                        auto itemObjectId=new QTableWidgetItem();
                        itemObjectId->setText(objectId);
                        auto itemPropertyName=new QTableWidgetItem();
                        itemPropertyName->setText(attribute);
                        auto itemValue=new QTableWidgetItem();
                        itemValue->setText(value);
                        ui->tableWidget->setItem(rowPos,0,itemKfNum);
                        ui->tableWidget->setItem(rowPos,1,itemObjectId);
                        ui->tableWidget->setItem(rowPos,2,itemPropertyName);
                        ui->tableWidget->setItem(rowPos,3,itemValue);
                        ui->tableWidget->blockSignals(false);

                    }
                }
            }
        }
    }
    else{
        QMessageBox::warning(this, tr("VecFX"),
                                       tr("You haven't opened an SVG. Open one first"),
                                       QMessageBox::Ok);
    }
}

void MainWindow::render(){
    // Accepts the folder where the files need to be written and writes the SVGs.
    QString writeDirectory=QFileDialog::getExistingDirectory(this, tr("Open Write Directory"),
                                                      "/home",
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    qInfo() << "Folder name:" << writeDirectory ;
    mainSVGObject->render(writeDirectory.toLocal8Bit().constData());
}
