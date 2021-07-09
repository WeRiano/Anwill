#pragma once

#include "Anwill.h"

class Layer2D : public Anwill::Layer
{
public:
    Layer2D(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;
};