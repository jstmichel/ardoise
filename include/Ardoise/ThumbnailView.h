#pragma once
#include <QListView>
namespace Ardoise
{
class ThumbnailView : public QListView
{
    Q_OBJECT
public:
    explicit ThumbnailView(QWidget* parent = nullptr);
};
}
