#ifndef JSVALUETOFUNCTION_H
#define JSVALUETOFUNCTION_H

#include "Tatami_global.h"
#include <functional>
#include <QJSValue>

std::function<void()> TATAMI_EXPORT jsValueToCallback(QJSValue callback);

#endif // JSVALUETOFUNCTION_H
