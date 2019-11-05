#pragma once

#ifndef QCEFCLIENT_H
#define QCEFCLIENT_H

#include <QString>

bool IsSubprocess(int & argc, char ** argv);

int QCefInit(int &argc, char ** argv);

void QCefQuit();

QString AppGetWorkingDirectory();

#endif // QCEFCLIENT_H
