#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include "singleton.h"
#include <QString>
#include "project_data.h"

struct GlobalData {
    QString project_path;
    Project_data data;
};

#endif // GLOBALDATA_H
