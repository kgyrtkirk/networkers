#ifndef VERTEX2D_INCLUDED
#define VERTEX2D_INCLUDED

/*
Structure to define two-dimensional vertices with arbitrary numbers of interpolated components
*/

template <int numIcsParam>
struct Vertex2D
	{
	/* Embedded classes: */
	static const int numIcs=numIcsParam; // Number of interpolated components in vertex
	
	/* Elements: */
	public:
	float pos[2]; // 2D position of vertex in device coordinates
	float ics[numIcs]; // Interpolation components of vertex
	};

#endif
