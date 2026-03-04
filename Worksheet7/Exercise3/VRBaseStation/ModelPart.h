/** @file ModelPart.h
 *
 * EEEE2076 - Software Engineering & VR Project
 *
 * Template for model parts that will be added as treeview items
 *
 * P Evans 2022
 */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

/* VTK headers - will be needed when VTK used in next worksheet,
 * commented out for now.
 */
//#include <vtkSmartPointer.h>
//#include <vtkMapper.h>
//#include <vtkActor.h>
//#include <vtkSTLReader.h>
//#include <vtkColor.h>

class ModelPart {
public:
    /** Constructor
     * @param data is a List (array) of strings for each property of this item
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor
      * Needs to free array of child items
      */
    ~ModelPart();

    /** Add a child to this item.
      * @param item Pointer to child object (must already be allocated using new)
      */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item
      * @param row is the row number (below this item)
      * @return pointer to the item requested.
      */
    ModelPart* child(int row);

    /** Return number of children to this item
      * @return number of children
      */
    int childCount() const;

    /** Get number of data items (2 - part name and visibility string)
      * @return number of visible data columns
      */
    int columnCount() const;

    /** Return the data item at a particular column for this item.
      * @param column is column index
      * @return the QVariant (represents string)
      */
    QVariant data(int column) const;

    /** Step 8.4: Add setData() to allow updating part properties
      * @param column is the index of the property to set
      * @param value is the value to apply
      */
    void setData(int column, QVariant value);

    /** Existing set function used by some implementations */
    void set(int column, const QVariant &value);

    /** Get pointer to parent item
      * @return pointer to parent item
      */
    ModelPart* parentItem();

    /** Get row index of item, relative to parent item
      * @return row index
      */
    int row() const;

    /** Step 9.4: Set colour (RGB values as ints) */
    void setColour(int r, int g, int b);

    /** Step 9.4: Getter methods for colour */
    int getColourR();
    int getColourG();
    int getColourB();

    /** Step 9.4: Set visible flag */
    void setVisible(bool visible);

    /** Step 9.4: Get visible flag */
    bool getVisible();

    /** Load STL file */
    void loadSTL(QString fileName);

private:
    QList<ModelPart*>           m_childItems;  /**< List (array) of child items */
    QList<QVariant>             m_itemData;    /**< List (array) of column data for item */
    ModelPart* m_parentItem;  /**< Pointer to parent */

    /** Step 9.4: Private member variables for storage */
    int red;
    int green;
    int blue;
    bool isVisible;
};

#endif
