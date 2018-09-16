/*!
 * \file validator.h
 */
#pragma once

/*!
 * \brief todo
 */
class validator
{
public:
    virtual ~validator();

    validator(const validator&) = delete;
    validator& operator=(const validator&) = delete;

    void validate();

protected:
    validator();

private:
    virtual void do_validate() = 0;
};
