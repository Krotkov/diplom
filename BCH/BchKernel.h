#pragma once

#include <Matrix/Matrix.h>
#include <Polynom/Polynom.h>

std::vector<Polynom<int>> createField(int n);

Matrix createExtendedBchKernel(int n);
