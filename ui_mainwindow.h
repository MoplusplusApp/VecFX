/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_SVG;
    QAction *actionOpen_Keyframe_File;
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *keyframeDataLayout;
    QLabel *label;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *addRowButton;
    QPushButton *removeRowButton;
    QPushButton *renderButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *keyframeDataLayout_2;
    QLabel *label_2;
    QListView *listView;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1091, 785);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #222222;\n"
"color: #DDDDDD;\n"
""));
        actionOpen_SVG = new QAction(MainWindow);
        actionOpen_SVG->setObjectName(QString::fromUtf8("actionOpen_SVG"));
        actionOpen_Keyframe_File = new QAction(MainWindow);
        actionOpen_Keyframe_File->setObjectName(QString::fromUtf8("actionOpen_Keyframe_File"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(420, 20, 611, 561));
        keyframeDataLayout = new QVBoxLayout(verticalLayoutWidget);
        keyframeDataLayout->setObjectName(QString::fromUtf8("keyframeDataLayout"));
        keyframeDataLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        keyframeDataLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("color: #DDDDDD;\n"
""));
        label->setAlignment(Qt::AlignCenter);

        keyframeDataLayout->addWidget(label);

        tableWidget = new QTableWidget(verticalLayoutWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setStyleSheet(QString::fromUtf8("QTableWidget{\n"
"	show-decoration-selected: 1;\n"
"	selection-color: cyan;\n"
"	color: #DDDDDD;\n"
"}\n"
"\n"
"QTableWidget::item:selected:active:hover{\n"
"    background-color:#42f5ce;\n"
"	 color: #111111;\n"
"}\n"
"QTableWidget::item:selected:active:!hover{\n"
"    background-color: #8000f7; color: #4a2548;\n"
"}\n"
"QTableWidget::item:selected:!active{\n"
"    background-color:yellow; color: #1e5054;\n"
"}\n"
"QTableWidget::item:!selected:hover{\n"
"    background-color:#915d96; color: #111111;\n"
"}"));

        keyframeDataLayout->addWidget(tableWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addRowButton = new QPushButton(verticalLayoutWidget);
        addRowButton->setObjectName(QString::fromUtf8("addRowButton"));
        addRowButton->setStyleSheet(QString::fromUtf8("background-color: #111111;\n"
"color: #DDDDDD;"));

        horizontalLayout->addWidget(addRowButton);

        removeRowButton = new QPushButton(verticalLayoutWidget);
        removeRowButton->setObjectName(QString::fromUtf8("removeRowButton"));
        removeRowButton->setStyleSheet(QString::fromUtf8("background-color: #111111;\n"
"color: #DDDDDD;"));

        horizontalLayout->addWidget(removeRowButton);

        renderButton = new QPushButton(verticalLayoutWidget);
        renderButton->setObjectName(QString::fromUtf8("renderButton"));
        renderButton->setAutoFillBackground(false);
        renderButton->setStyleSheet(QString::fromUtf8("background-color: #111111;\n"
"color: #DDDDDD;"));

        horizontalLayout->addWidget(renderButton);


        keyframeDataLayout->addLayout(horizontalLayout);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(20, 20, 351, 561));
        keyframeDataLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        keyframeDataLayout_2->setObjectName(QString::fromUtf8("keyframeDataLayout_2"));
        keyframeDataLayout_2->setSizeConstraint(QLayout::SetMinAndMaxSize);
        keyframeDataLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("color: #DDDDDD;\n"
""));
        label_2->setAlignment(Qt::AlignCenter);

        keyframeDataLayout_2->addWidget(label_2);

        listView = new QListView(verticalLayoutWidget_2);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setStyleSheet(QString::fromUtf8("QListView{\n"
"	show-decoration-selected: 1;\n"
"	selection-color: cyan;\n"
"	color: #DDDDDD;\n"
"}\n"
"\n"
"QListView::item:selected:active:hover{\n"
"    background-color:#42f5ce;\n"
"	 color: #111111;\n"
"}\n"
"QListView::item:selected:active:!hover{\n"
"    background-color: #8000f7; color: #4a2548;\n"
"}\n"
"QListView::item:selected:!active{\n"
"    background-color:yellow; color: #1e5054;\n"
"}\n"
"QListView::item:!selected:hover{\n"
"    background-color:#915d96; color: #111111;\n"
"}"));

        keyframeDataLayout_2->addWidget(listView);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1091, 22));
        menubar->setStyleSheet(QString::fromUtf8("QMenuBar#QMenu{\n"
"	background-color: rgb(193, 125, 17);\n"
"	color: #000000;\n"
"}\n"
"QMenuBar#QMenu:hover{\n"
"	background-color:#000000;\n"
"	color:  rgb(193, 125, 17);\n"
"}"));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionOpen_SVG);
        menuFile->addAction(actionOpen_Keyframe_File);

        retranslateUi(MainWindow);
        QObject::connect(renderButton, SIGNAL(clicked()), MainWindow, SLOT(render()));
        QObject::connect(tableWidget, SIGNAL(cellChanged(int,int)), MainWindow, SLOT(cellChangedTable(int,int)));
        QObject::connect(addRowButton, SIGNAL(clicked()), MainWindow, SLOT(rowAdded()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_SVG->setText(QCoreApplication::translate("MainWindow", "Open SVG", nullptr));
        actionOpen_Keyframe_File->setText(QCoreApplication::translate("MainWindow", "Open Keyframe File", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Keyframe Data", nullptr));
        addRowButton->setText(QCoreApplication::translate("MainWindow", "Add Row", nullptr));
        removeRowButton->setText(QCoreApplication::translate("MainWindow", "Remove Row", nullptr));
        renderButton->setText(QCoreApplication::translate("MainWindow", "Render", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "SVG Hiraerchy Viewer", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
