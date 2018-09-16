/*!
 * \file property_validator.h
 */
#pragma once

#include "validator.h"

/*!
 * \brief todo
 */
class property_validator : validator
{
public:
    property_validator();

private:
    void do_validate() override;
};
