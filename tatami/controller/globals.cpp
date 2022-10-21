#include "globals.h"

TatamiGlobalState* TatamiGlobalState::instance = nullptr;

TatamiGlobalState::TatamiGlobalState(QObject* parent) : QObject(parent)
{
  if (instance != nullptr)
    throw std::runtime_error("TatamiGlobalState was initialized twice.");
  instance = this;
}

TatamiGlobalState::~TatamiGlobalState()
{
  instance = nullptr;
}
