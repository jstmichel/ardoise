#pragma once
#include <QLineEdit>
namespace Ardoise
{
class SearchField : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchField(QWidget* parent = nullptr);
};
}
