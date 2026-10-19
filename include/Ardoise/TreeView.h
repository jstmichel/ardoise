#pragma once
#include <QTreeView>
namespace Ardoise
{
class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget* parent = nullptr);
};
}
