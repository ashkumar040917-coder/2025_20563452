/** @file ModelPart.cpp
  *
  * EEEE2076 - Software Engineering & VR Project
  *
  * Template for model parts that will be added as treeview items
  *
  * P Evans 2022
  */

#include "ModelPart.h"

/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
//#include <vtkSmartPointer.h>
//#include <vtkDataSetMapper.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent) {

    // Step 9.4: Initialise colour and visibility defaults
    red = 0;
    green = 0;
    blue = 0;
    isVisible = true;
}

ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}

int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size())
        return QVariant();

    // Column 0 is the Name (from the itemData list)
    if (column == 0) {
        return m_itemData.at(column);
    }

    // Column 1 is Visibility
    // We return "true" or "false" based on our internal bool
    if (column == 1) {
        return isVisible ? "true" : "false";
    }

    return QVariant();
}

void ModelPart::setData(int column, QVariant value) {
    /* Step 8.4: Set the data associated with a column of this item
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

void ModelPart::set(int column, const QVariant &value) {
    /* Existing set function used by some implementations
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

/** Step 9.4: Implementation of Colour and Visibility methods */

void ModelPart::setColour(int r, int g, int b) {
    red   = r;
    green = g;
    blue  = b;
}

int ModelPart::getColourR() { return red; }
int ModelPart::getColourG() { return green; }
int ModelPart::getColourB() { return blue; }

void ModelPart::setVisible(bool visible) {
    isVisible = visible;
}

bool ModelPart::getVisible() {
    return isVisible;
}

/** Placeholder for Exercise 11+ */
void ModelPart::loadSTL(QString fileName) {
    /* This is a placeholder function that you will need to modify if you want to use it */

    /* 1. Use the vtkSTLReader class to load the STL file
     * https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */

    /* 2. Initialise the part's vtkMapper */

    /* 3. Initialise the part's vtkActor and link to the mapper */
}

//vtkSmartPointer<vtkActor> ModelPart::getActor() {
/* Needs to return a smart pointer to the vtkActor to allow
     * part to be rendered.
     */
//}

//vtkActor* ModelPart::getNewActor() {
/* 1. Create new mapper */

/* 2. Create new actor and link to mapper */

/* 3. Link the vtkProperties of the original actor to the new actor. */
//}
