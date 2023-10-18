#ifndef NSUMO_APP_H
#define NSUMO_APP_H

#include "Application.h"

class NsumoApp : public Application
{
public:
    NsumoApp();
    ~NsumoApp();
    void onKeyEvent(const Event::Key &keyEvent) override;
    void onFixedUpdate() override;
};

#endif // NSUMO_APP_H
