#pragma once

#include <cxcore.h>
#include "tools.h"

/** Describes a thin wrapper for the cv::Point_ class that adds some extra functionality */
template <class T>
class PointPlus : public cv::Point_<T>
{
public:
    /** Creates a PointPlus */
    PointPlus() : cv::Point_<T>(){}

    /** Creates a PointPlus from an (x,y) coordinate */
    PointPlus( T x, T y ) : cv::Point_<T>(x,y){}

    /** Creates a PointPlus from a standard cv::Point_ */
    PointPlus( const cv::Point_<T>& p ) : cv::Point_<T>(p){}

    /** Creates a PointPlus from a standard cv::Point */
    PointPlus( const cv::Point& p ) : cv::Point_<T>(p.x,p.y){}

    /** Performs an affine transformation on this PointPlus using a transformation matrix */
    void transform( const cv::Mat& xfm )
    {
        // Creates a matrix equivalent of the point and transforms it
        cv::Mat_<T> pt( cv::Size(1,3) );
        pt[0][0] = this->x;
        pt[1][0] = this->y;
        pt[2][0] = 1.0;
        pt = xfm * pt;
        this->x = pt[0][0];
        this->y = pt[1][0];
    }

    /** Calculates the distance between this and another point */
    inline double dist( const PointPlus<T>& other )
    {
        return sqrt( pow( this->x - other.x, 2.0 ) + pow( this->y - other.y, 2.0 ) );
    }

    /** Adds two PointPlus together to form a new one */
    PointPlus<T> operator+( const PointPlus<T>& other )
    {
        PointPlus<T> neo = *this;
        neo.x += other.x;
        neo.y += other.y;
        return neo;
    }

    /** Moves this point by a scalar x and y distance */
    void move( T dx, T dy )
    {
        this->x += dx;
        this->y += dy;
    }

    /** Returns a string representation of this PointPlus */
    inline std::string str()
    {
        return std::string( "( " + num2str(this->x) + ", " + num2str(this->y) + " )" );
    }

    /** Allows the string representation of a PointPlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, PointPlus& p )
    {
        return out << p.str();
    }
};

/** Describes a thin wrapper for the cv::Size_ class that adds some extra functionality */
template <class T>
class SizePlus : public cv::Size_<T>
{
public:
    /// @note The w and h reference variables are merely to conveniently acces width and height
    T& w;
    T& h;

    /** Creates a SizePlus */
    SizePlus() : cv::Size_<T>(), w(this->width), h(this->height){}

    /** Creates a SizePlus from a width and height */
    SizePlus( T width, T height ) : cv::Size_<T>(width,height), w(this->width), h(this->height){}

    /** Creates a SizePlus from a standard cv::Size_ */
    SizePlus( const cv::Size_<T>& s ) : cv::Size_<T>(s), w(this->width), h(this->height){}

    /** Creates a new SizePlus that is equivalent to this SizePlus scaled by a constant scale factor */
    SizePlus<T> operator*( double scale )
    {
        SizePlus<T> neo = *this;
        neo.w *= scale;
        neo.h *= scale;
        return neo;
    }

    /** Scales this SizePlus by a constant scale factor */
    SizePlus<T>& operator*=( double scale )
    {
        this->w *= scale;
        this->h *= scale;
        return *this;
    }

    /** Returns a string representation of this SizePlus */
    inline std::string str()
    {
        return std::string( "( " + num2str(this->w) + ", " + num2str(this->h) + " )" );
    }

    /** Allows the string representation of a SizePlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, SizePlus& s )
    {
        return out << s.str();
    }
};

/** Describes a thin wrapper for the cv::Rect_ class that adds some extra functionality */
template <class T>
class RectPlus : public cv::Rect_<T>
{
public:
    /** Creates a PointPlus */
    RectPlus() : cv::Rect_<T>(){}

    /** Creates a RectPlus from an (x,y,w,h) rectangle description */
    RectPlus( T x, T y, T w, T h ) : cv::Rect_<T>(x,y,w,h){}

    /** Creates a RectPlus from a standard template cv::Rect_ */
    RectPlus( const cv::Rect_<T>& r ) : cv::Rect_<T>(r){}

    /** Creates a RectPlus from a standard general cv::Rect */
    RectPlus( const cv::Rect& r ) : cv::Rect_<T>( (T)r.x, (T)r.y, (T)r.width, (T)r.height ){}

    /** Creates a RectPlus from an old style CvRect */
    RectPlus( const CvRect& r ) : cv::Rect_<T>(r){}

    /** Creates a RectPlus from a SizePlus object */
    RectPlus( const SizePlus<T>& sz ) : cv::Rect_<T>(0,0,sz.width,sz.height){}

    /** Creates a RectPlus from a cv::Size_ object */
    RectPlus( const cv::Size_<T>& sz ) : cv::Rect_<T>(0,0,sz.width,sz.height){}

    /** Creates a RectPlus from an upper-left and lower-left point */
    RectPlus( const cv::Point_<T>& ul, const cv::Point_<T>& lr )
    {
        this->x = ul.x;
        this->y = ul.y;
        this->width = lr.x - ul.x;
        this->height = lr.y - ul.y;
    }

    /** Finds and returns the center of this RectPlus as a PointPlus */
    inline PointPlus<T> center()
    {
        return PointPlus<T>( this->x + this->width / 2, this->y + this->height / 2 );
    }

    /** Returns a the upper-left corner of this RectPlus */
    inline PointPlus<T> ul()
    {
        return PointPlus<T>( this->x, this->y );
    }

    /** Returns a the lower-left corner of this RectPlus */
    inline PointPlus<T> ll()
    {
        return PointPlus<T>( this->x, this->y + this->height );
    }

    /** Returns a the upper-right corner of this RectPlus */
    inline PointPlus<T> ur()
    {
        return PointPlus<T>( this->x + this->width, this->y );
    }

    /** Returns a the lower-right corner of this RectPlus */
    inline PointPlus<T> lr()
    {
        return PointPlus<T>( this->x + this->width, this->y + this->height );
    }

    /** Returns the x position of the left side of this RectPlus */
    inline T left()
    {
        return this->x;
    }

    /** Returns the y position of the top side of this RectPlus */
    inline T top()
    {
        return this->y;
    }

    /** Returns the x position of the right side of this RectPlus */
    inline T right()
    {
        return this->x + this->width;
    }

    /** Returns the y position of the bottom side of this RectPlus */
    inline T bottom()
    {
        return this->y + this->height;
    }

    /** Moves this RectPlus so that it is centered on a point */
    void centerOn( const cv::Point_<T>& pt )
    {
        this->x += pt.x - this->center().x;
        this->y += pt.y - this->center().y;
    }

    /** Anchors this RectPlus's upper-left coner on a point */
    void anchorOn( const cv::Point_<T>& pt )
    {
        this->x = pt.x;
        this->y = pt.y;
    }

    /** Moves this RectPlus by a scalar x and y distance */
    void move( T dx, T dy )
    {
        this->x += dx;
        this->y += dy;
    }

    /** Returns a cv::RotatedRect from this rectangle */
    inline void rotated( double angle )
    {
        return cv::RotatedRect( this->center(), this->size(), angle );
    }

    /** Scales this RectPlus by a constant scale factor relative to a designated origin point */
    void scale( double scl, PointPlus<T> origin=PointPlus<T>() )
    {
        this->x = origin.x + ( this->x - origin.x ) * scl;
        this->y = origin.y + ( this->y - origin.y ) * scl;
        this->width *= scl;
        this->height *= scl;
    }

    /** Scales this RectPlus by a constant scale factor relative to its own center */
    RectPlus& operator*=( double scl )
    {
        this->scale( scl, this->center() );
        return *this;
    }

    /** Returns a string representation of this RectPlus */
    inline std::string str()
    {
        return std::string( "[ " + num2str(this->x)     + ", " + num2str(this->y)      + ", " +
                              num2str(this->width) + ", " + num2str(this->height) + " ]" );
    }


    /** Allows the string representation of a RectPlus to be placed on an output stream */
    friend std::ostream& operator<<( std::ostream& out, RectPlus& r )
    {
        return out << r.str();
    }

};
