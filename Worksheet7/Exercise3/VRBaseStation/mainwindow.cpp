#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "optiondialog.h"
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- VTK INITIALIZATION ---
    // Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    // --- CYLINDER ---
    // Create cylinder source
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // Create mapper
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // Create actor
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    // Add actor to renderer
    renderer->AddActor(cylinderActor);

    // Set up camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    // Force render
    renderWindow->Render();

    // ------------------------------------------------

    // Link UI objects to our custom handler functions
    connect(ui->pushButton,   &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Link our custom signal to the built-in showMessage slot of the statusbar
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);

    // Create the ModelList
    ui->treeView->addAction(ui->actionItem_Options);
    this->partList = new ModelPartList("PartsList");

    // Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // Manually create a model tree
    ModelPart *rootItem = this->partList->getRootItem();

    // Add 3 top level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        ModelPart *childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart *childChildItem = new ModelPart({ name, visible });
            childItem->appendChild(childChildItem);
        }
    }

    // Capture the clicked signal emitted when a user clicks on the Tree View
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- SLOT IMPLEMENTATIONS ---

void MainWindow::handleButton1() {
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::handleButton2() {
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected — click a tree item first"), 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (selectedPart == nullptr) {
        return;
    }

    OptionDialog dialog(this);
    dialog.loadFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToModelPart(selectedPart);
        emit partList->dataChanged(index, index);
        emit statusUpdateMessage(QString("Item updated: ") + selectedPart->data(0).toString(), 0);
    } else {
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}

void MainWindow::handleTreeClicked() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (selectedPart == nullptr) return;
    QString text = selectedPart->data(0).toString();
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );

    if (fileName.isEmpty()) {
        return;
    }

    QModelIndex index = ui->treeView->currentIndex();
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        if (selectedPart != nullptr) {
            selectedPart->setData(0, QVariant(fileName));
            emit partList->dataChanged(index, index);
        }
    }
    emit statusUpdateMessage(QString("Opened: ") + fileName, 0);
}

void MainWindow::on_actionItem_Options_triggered() {
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected"), 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (selectedPart == nullptr) {
        return;
    }

    OptionDialog dialog(this);
    dialog.loadFromModelPart(selectedPart);

    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToModelPart(selectedPart);
        emit partList->dataChanged(index, index);
        emit statusUpdateMessage(
            QString("Item Options saved: ") + selectedPart->data(0).toString(), 0
            );
    } else {
        emit statusUpdateMessage(QString("Item Options cancelled"), 0);
    }
}
