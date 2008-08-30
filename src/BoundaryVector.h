#ifndef _BOUNDARYVECTOR_H_
#define _BOUNDARYVECTOR_H_

#include <blitz/array.h>
#include "Direction.h"

BZ_USING_NAMESPACE(blitz)

namespace ibpm {
    
/*!
    \file BoundaryVector.h
    \class BoundaryVector

    \brief Store vectors located at boundary points
    
    Examples are forces, velocities, and coordinates of points on a body.

    \author Clancy Rowley
    \author $LastChangedBy$
    \date  7 Jul 2008
    \date $LastChangedDate$
    \version $Revision$
*/

class BoundaryVector {
public:
    /// \brief Constructor, allocating memory for a body with
    /// n points on the boundary.
    BoundaryVector(int n) :
        _numPoints(n),
        _data( _numPoints * XY )
        {};

    /// Allocate a new BoundaryVector, copy the data
    inline BoundaryVector(const BoundaryVector& f) :
        _numPoints(f._numPoints),
        _data( _numPoints * XY ) {
        _data = f._data;
    }

    /*! \brief Constructor using pre-existing data, as a 1d array.

    Here, n is the number of boundary points, and *data has size 2n.
    */
    BoundaryVector(int n, double* data);
    // TODO: Implement this, and write tests

    /// Return the number of boundary points
    inline int getNumPoints() { return _numPoints; }
    
    /*! \brief Return the number of elements in the array.
    
    (twice the number of boundary points)
    */
    inline int getSize() { return XY*_numPoints; }
    
    // Print the contents to standard output, for debugging
    inline void print() {
        cout << _data << endl;
    }
    
    /// f(dir,i) refers to the value in direction dir (X or Y) at point i
    inline double& operator()(int dir, int i) {
        assert(dir>=X  && dir<=Y);
        assert(i>=0  && i<_numPoints);
        return _data(dir * _numPoints + i);
    }
    
    /// f(dir,i) refers to the value in direction dir (X or Y) at point i
    inline double operator()(int dir, int i) const {
        assert(dir>=X  && dir<=Y);
        assert(i>=0  && i<_numPoints);
        return _data(dir * _numPoints + i);
    }
    
    /// Type used for referencing elements
    typedef int index;
    
    /// f(ind) refers to the value corresponding to the given index ind
    inline double& operator()(index ind) {
        assert( (ind >= 0) && (ind < _numPoints * XY) );
        return _data(ind);
    }

    /// f(ind) refers to the value corresponding to the given index ind
    inline double operator()(index ind) const {
        assert( (ind >= 0) && (ind < _numPoints * XY) );
        return _data(ind);
    }

    /// Returns an index that refers to the first element
    inline index begin() const { return 0; }

    /// Returns an index that is one past the last element
    inline index end() const { return _numPoints * XY; }

    /// Returns an index for the first element in direction dir (X or Y)
    inline index begin(int dir) const {
        assert ((dir >= X) && (dir <= Y));
        return dir * _numPoints;
    }

    /// Returns an index one past the last element in direction dir (X or Y)
    inline index end(int dir) const {
        assert ((dir >= X) && (dir <= Y));
        return (dir+1) * _numPoints;
    }
    
    /// Returns an index for the value in direction dir at point i
    inline index getIndex(int dir, int i) const {
        assert(dir>=X  && dir<=Y);
        assert(i>=0  && i<_numPoints);
        return dir * _numPoints + i;
    }
    
    /// Return a pointer to the data, expressed as a C-style array.
    double* flatten();
    
    /// Return the dot product of *this and the argument
    double dot(BoundaryVector& f);
    // TODO: Implement this, and write tests

    /// Copy assignment
    inline BoundaryVector& operator=(const BoundaryVector& f) {
        assert(f._numPoints == _numPoints);
        _data = f._data;
        return *this;
    };

    /// Copy assignment from double
    inline BoundaryVector& operator=(double a) {
        _data = a;
        return *this;
    };

    /// f += g
    inline BoundaryVector& operator+=(const BoundaryVector& f) {
        assert(f._numPoints == _numPoints);
        _data += f._data;
        return *this;
    }

    /// f -= g
    inline BoundaryVector& operator-=(const BoundaryVector& f) {
        assert(f._numPoints == _numPoints);
        _data -= f._data;
        return *this;
    }

    /// f *= a
    inline BoundaryVector& operator*=(double a) {
        _data *= a;
        return *this;
    }
    
    /// f /= a
    inline BoundaryVector& operator/=(double a) {
        _data /= a;
        return *this;
    }
    
    /// f + g
    inline BoundaryVector operator+(const BoundaryVector& g) {
        BoundaryVector h = *this;
        h += g;
        return h;
    }
    
    /// f - g
    inline BoundaryVector operator-(const BoundaryVector& g) {
        BoundaryVector h = *this;
        h -= g;
        return h;
    }
    
    /// f * a
    inline BoundaryVector operator*(double a) {
        BoundaryVector g = *this;
        g *= a;
        return g;
    }
    
    /// f / a
    inline BoundaryVector operator/(double a) {
        BoundaryVector g = *this;
        g /= a;
        return g;
    }

    friend double InnerProduct(BoundaryVector& x, BoundaryVector& y);
    
private:
    int _numPoints;
    Array<double,1> _data;
};  // class BoundaryVector

/// -f
inline BoundaryVector operator-(const BoundaryVector& f) {
    BoundaryVector g = f;
    g *= -1;
    return g;
}

/// a * f
inline BoundaryVector operator*(double a, const BoundaryVector& f) {
    BoundaryVector g = f;
    g *= a;
    return g;
}

/// Return the inner product of BoundaryVectors x and y.
inline double InnerProduct(BoundaryVector& x, BoundaryVector& y) {
    // // Implementation using only public interface
    // BoundaryVector::index ind;
    // double ip = 0;
    // 
    // for (ind = x.begin(); ind != x.end(); ++ind) {
    //     ip += x(ind) * y(ind);
    // }    
    // return ip;
    
    // Implementation using Blitz arrays
    return sum(x._data * y._data);
}

} // namespace ibpm

#endif /* _BOUNDARYVECTOR_H_ */
