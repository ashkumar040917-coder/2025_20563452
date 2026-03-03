#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QFileDialog>

#include "optiondialog.h"


MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent)

    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);

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

    // Get the currently selected tree item
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected — click a tree item first"), 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (selectedPart == nullptr) {
        return;
    }

    // Create the dialog as a child of this window
    OptionDialog dialog(this);

    // Populate the dialog fields from the selected part's current data
    dialog.loadFromModelPart(selectedPart);

    // Show the dialog and wait for user response
    if (dialog.exec() == QDialog::Accepted) {

        // User clicked OK — save the dialog values back into the ModelPart
        dialog.saveToModelPart(selectedPart);

        // Tell the tree view to redraw this item
        emit partList->dataChanged(index, index);

        emit statusUpdateMessage(QString("Item updated: ") + selectedPart->data(0).toString(), 0);

    } else {
        // User clicked Cancel
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}

void MainWindow::handleTreeClicked() {

    // Get the index of the selected item

    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the item from the index

    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name string from the internal QVariant data array

    QString text = selectedPart->data(0).toString();

    // Emit statusUpdateMessage with the selected item text

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);

}
void MainWindow::on_actionOpen_File_triggered() {

    // Open the file dialog and store the selected filename
    QString fileName = QFileDialog::getOpenFileName(
        this,                              // parent widget
        tr("Open File"),                   // dialog title
        "C:\\",                            // default directory
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*.*)")
        );

    // If the user cancelled, fileName will be empty — do nothing
    if (fileName.isEmpty()) {
        return;
    }

    // Get whichever tree item is currently selected
    QModelIndex index = ui->treeView->currentIndex();

    if (index.isValid()) {
        // Cast the internal pointer to get the ModelPart
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        if (selectedPart != nullptr) {
            // Update the name of the selected part to the filename
            selectedPart->setData(0, QVariant(fileName));

            // Tell the view that this item's data has changed so it redraws
            emit partList->dataChanged(index, index);
        }
    }

    // Show the filename in the status bar
    emit statusUpdateMessage(QString("Opened: ") + fileName, 0);
}
void MainWindow::on_actionItem_Options_triggered() {

    // Get the currently selected item in the tree
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected"), 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (selectedPart == nullptr) {
        return;
    }

    // Create and populate the dialog
    OptionDialog dialog(this);
    dialog.loadFromModelPart(selectedPart);

    // Show dialog and handle response
    if (dialog.exec() == QDialog::Accepted) {

        // Save changes back into the ModelPart
        dialog.saveToModelPart(selectedPart);

        // Refresh the tree view display
        emit partList->dataChanged(index, index);

        emit statusUpdateMessage(
            QString("Item Options saved: ") + selectedPart->data(0).toString(), 0
            );

    } else {
        emit statusUpdateMessage(QString("Item Options cancelled"), 0);
    }
}
