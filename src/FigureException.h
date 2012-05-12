#ifndef _FIGURE_EXCEPTION_H_
#define _FIGURE_EXCEPTION_H_

#include <exception>

class IntersectorException : public std::exception
{
};

class IntersectionNotSupported : public IntersectorException
{
};

#endif
