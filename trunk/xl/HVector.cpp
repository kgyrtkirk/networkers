/***********************************************************************
HVector - Class for 3-dimensional homogenuous vectors. (Simplified
version of HVector class from the Templatized Geometry Library.)
Copyright (c) 2005 Oliver Kreylos

This file is part of the Templatized Geometry Library (TGL).

The Templatized Geometry Library is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The Templatized Geometry Library is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with the Templatized Geometry Library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA
***********************************************************************/

#include "HVector.h"

namespace Geometry {

/*******************************
Static elements of class HVector:
*******************************/

const HVector HVector::zero(0.0f,0.0f);
const HVector HVector::origin(0.0f,1.0f);

}
